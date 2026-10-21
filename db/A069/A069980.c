/*
 * A069980  (Sum of digits of n)^6 - (sum of digits of n^6).
 * https://oeis.org/A069980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn, look
 * Formula:   a(n) = (digitSum(n))^6 - digitSum(n^6)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* Sum of digits of n (using the provided helper) */
    seq_t ds = digit_sum((seq_t)n);

    /* (digitSum(n))^6  */
    seq_t ds_pow = ds;
    for (int i = 1; i < 6; ++i) ds_pow *= ds;

    /* Compute n^6 using 128‑bit arithmetic to avoid overflow */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 n6 = nn * nn * nn * nn * nn * nn;

    /* Sum of digits of n^6 (digit sum on the 128‑bit value) */
    seq_t ds6 = 0;
    unsigned __int128 tmp = n6;
    while (tmp) {
        ds6 += (seq_t)(tmp % 10);
        tmp /= 10;
    }

    /* Final result */
    return ds_pow - ds6;
}
