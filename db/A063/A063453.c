/*
 * A063453  Multiplicative with a(p^e) = 1 - p^3.
 * https://oeis.org/A063453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, sign
 * Formula:   a(n) = product_{p∣n} (1 - p^3) with a(1) = 1 Uses the factor_u64
 *            routine from the utils directory
 * Range:     i8 n <= 5, i16 n <= 32, i32 n <= 1290, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(n, primes, exps, &cnt);   /* factorization of n */

    seq_t res = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t p = (seq_t)primes[i];
        seq_t p2 = p * p;
        seq_t p3 = p2 * p;              /* p^3 */
        res *= ((seq_t)1 - p3);
    }
    return res;
}
