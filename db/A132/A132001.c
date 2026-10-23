/*
 * A132001  Expansion of 1 - (1/3) * b(q) * b(q^2) * c(q)^2 / c(q^2) in powers
 *          of q where b(), c() are cubic AGM functions.
 * https://oeis.org/A132001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = ∏_{p^e‖n} a(p^e) where a(2^e) = 2 + ((-4)^(e+1) - 1)/5
 *            a(3^e) = 1 a(p^e) = ((p^2 * kr)^ (e+1) - 1) / (p^2 * kr - 1) for
 *            odd p>3 with kr = Kronecker(-3,p) = 1 if p≡1 (mod 3), -1 if p≡2
 *            (mod 3)
 * Range:     i8 n <= 4, i16 n <= 16, i32 n <= 3500, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 but guard */
    size_t cnt = 0;
    uint64_t primes[64];
    uint64_t exps[64];
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t term = 1;

        if (p == 2) {
            /* a(2^e) = 2 + ((-4)^(e+1) - 1)/5 */
            seq_t pow_val = big_pow((seq_t)(-4), (seq_t)(e + 1));
            term = 2 + ((pow_val - 1) / 5);
        } else if (p == 3) {
            /* a(3^e) = 1 */
            term = 1;
        } else {
            /* p > 3 */
            /* Kronecker(-3, p) = 1 if p % 3 == 1 else -1 */
            seq_t p_sq = (seq_t)p * (seq_t)p;
            seq_t q = p_sq;
            if (p % 3 == 2) q = -q;

            seq_t pow_val = big_pow(q, (seq_t)(e + 1));
            term = (pow_val - 1) / (q - 1);
        }
        result *= term;
    }
    return result;
}
