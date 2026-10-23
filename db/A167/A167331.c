/*
 * A167331  Totally multiplicative sequence with a(p) = 2*(3p-1) = 6p-2 for
 *          prime p.
 * https://oeis.org/A167331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} (2*(3p-1))^e ; a(1)=1
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, seq_mul, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_one.h"

seq_t a(idx_t n)
{
    /* 1 is the result for n == 1 */
    if (n == 1)
        return seq_one();

    /* Factor n into primes and exponents. */
    /* 64 is enough for any number of prime factors of a 64-bit integer. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = seq_one();

    for (size_t k = 0; k < cnt; ++k) {
        /* base = 2 * (3p - 1) */
        seq_t base = (seq_t)2 * (((seq_t)3 * (seq_t)primes[k]) - (seq_t)1);
        /* Multiply result by base^exps[k] */
        for (idx_t e = 0; e < exps[k]; ++e) {
            result = seq_mul(result, base);
        }
    }

    return result;
}
