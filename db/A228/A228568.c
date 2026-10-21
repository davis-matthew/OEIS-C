/*
 * A228568  a(n) = 2^n*A056236(n).
 * https://oeis.org/A228568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 8*a(n-2), with a(0)=2, a(1)=8
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses only arithmetic on seq_t, avoiding
   dynamic allocation to keep the routine valid for any seq_t implementation. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)8;

    seq_t prev2 = (seq_t)2;   /* a(0) */
    seq_t prev1 = (seq_t)8;   /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = 8*a(i-1) - 8*a(i-2) = (a(i-1)-a(i-2)) * 8 */
        cur = (prev1 - prev2) * (seq_t)8;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
