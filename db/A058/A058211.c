/*
 * A058211  Numbers k such that there is an arrangement of k points in the
 *          plane such that no point is on >= k/2 connecting lines.
 * https://oeis.org/A058211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, hard, more
 * Formula:   a(n) = 6*n+1 for n >= 3; a(1)=9; a(2)=15
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }
    if (n == 1) {
        return (seq_t)9;
    }
    if (n == 2) {
        return (seq_t)15;
    }
    /* for n >= 3 */
    return (seq_t)6 * n + (seq_t)1;
}
