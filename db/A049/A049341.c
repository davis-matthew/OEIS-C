/*
 * A049341  a(n+1) = sum of digits of a(n) + a(n-1).
 * https://oeis.org/A049341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(0) = 3, a(1) = 6, a(n) = digit_sum(a(n-1) + a(n-2)) for n >= 2
 *            (offset 0)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    if (n == 0ULL) return (seq_t)3;
    if (n == 1ULL) return (seq_t)6;

    seq_t a_nm2 = (seq_t)3;        /* a(0) */
    seq_t a_nm1 = (seq_t)6;        /* a(1) */
    seq_t a_n   = a_nm1;           /* placeholder */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t sum = a_nm1 + a_nm2;
        a_n = digit_sum(sum);
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
