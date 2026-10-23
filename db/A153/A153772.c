/*
 * A153772  a(n) = (2^n + 2*(-1)^n - 6)/3.
 * https://oeis.org/A153772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0) = -1, a(1) = -2, a(n) = a(n-1) + 2*a(n-2) + 4 for n ≥ 2
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 32, i64 n <= 64, i128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0) return (seq_t)-1;
    if (n == 1) return (seq_t)-2;

    /* Start from the known values and iterate up to n. */
    seq_t prev2 = (seq_t)-1;   /* a(0) */
    seq_t prev1 = (seq_t)-2;   /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = a(i-1) + 2*a(i-2) + 4 */
        cur = prev1 + (seq_t)2 * prev2 + (seq_t)4;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
