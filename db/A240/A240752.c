/*
 * A240752  First differences of digit sums of squares, cf. A004159.
 * https://oeis.org/A240752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = digit_sum(n^2) – digit_sum((n-1)^2)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the sum of decimal digits of a uint64_t.
   The sum is small enough for seq_t even on 8‑bit builds. */
static inline seq_t digit_sum_uint64(uint64_t x)
{
    seq_t s = 0;
    while (x) {
        s += x % 10;
        x /= 10;
    }
    return s;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  Return 0 for n = 0 to
       avoid undefined behaviour when subtracting 1 from 0. */
    if (n == 0)
        return 0;

    /* Compute the squares using uint64_t to avoid overflow of seq_t. */
    uint64_t sq_n  = (uint64_t)n  * (uint64_t)n;
    idx_t     nm1  = n - 1;
    uint64_t sq_nm1 = (uint64_t)nm1 * (uint64_t)nm1;

    /* Digit sums of the two squares. */
    seq_t s_n  = digit_sum_uint64(sq_n);
    seq_t s_nm1 = digit_sum_uint64(sq_nm1);

    /* First difference of digit sums. */
    return s_n - s_nm1;
}
