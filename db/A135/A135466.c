/*
 * A135466  a(n) = (2*n-8)^2 * 2^(n-3).
 * https://oeis.org/A135466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-8)^2 * 2^(n-3)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 24, u64 n <= 53, u128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute 2*n-8 in signed 64‑bit */
    int64_t temp = (int64_t)2 * (int64_t)n - 8;
    /* absolute value of the difference */
    uint64_t abs_val = temp < 0 ? (uint64_t)(-temp) : (uint64_t)temp;

    /* square the absolute value */
    seq_t diff   = (seq_t)abs_val;
    seq_t diff_sq = diff * diff;

    /* multiply or divide by 2^(n-3) */
    if (n >= 3)
        return diff_sq << (n - 3);   /* 2^(n-3) */
    else
        return diff_sq >> (3 - n);   /* /2^(3-n) */
}
