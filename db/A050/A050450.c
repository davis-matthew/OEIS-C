/*
 * A050450  a(n) = Sum_{d|n, d=1 mod 4} d^2.
 * https://oeis.org/A050450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d ≡ 1 (mod 4)} d²
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_uint64, rec_div
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum of the squares of all divisors of n that are 1 mod 4. */

#include <stdint.h>
#include "../utils/factor_uint64.h"
#include "../utils/rec_div.h"

seq_t a(idx_t n)
{
    if (n == 0)                /* defined only for n ≥ 1 */
        return 0;

    uint64_t fac[20];
    int exp[20];
    int cnt;

    /* factor_uint64(n, fac, exp, &cnt) fills fac[0..cnt-1] with prime factors
       and exp[0..cnt-1] with corresponding exponents. */
    factor_uint64((uint64_t)n, fac, exp, &cnt);

    seq_t sum = 0;
    rec_div(0, (seq_t)1, fac, exp, cnt, &sum);
    return sum;
}
