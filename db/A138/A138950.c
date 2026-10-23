/*
 * A138950  Expansion of (2 - 3 * phi(q^3)^2 + phi(q)^2) / 4 in powers of q
 *          where phi() is a Ramanujan theta function.
 * https://oeis.org/A138950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) is multiplicative with * a(2^e) = 1 * a(3^e) = -1 +
 *            2*(-1)^e * a(p^e) (p≡1,5 mod12) = e+1 * a(p^e) (p≡7,11 mod12) =
 *            (1+(-1)^e)/2
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, return 0 */

    uint64_t N = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt;

    factor_u64(N, primes, exps, &cnt);

    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t term;

        if (p == 2) {
            term = 1;
        } else if (p == 3) {
            term = (e % 2 == 0) ? (seq_t)1 : (seq_t)(-3);
        } else {
            uint64_t mod = p % 12;
            if (mod == 1 || mod == 5) {
                term = (seq_t)(e + 1);
            } else if (mod == 7 || mod == 11) {
                term = (e % 2 == 0) ? (seq_t)1 : (seq_t)0;
            } else {
                /* This case should not occur for primes > 3. */
                term = 1;
            }
        }

        res *= term;
    }

    return res;
}
