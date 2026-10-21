/*
 * A202280  y-values in the solutions to x^2 - 151*y^2 = 1.
 * https://oeis.org/A202280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3456296080*a(n-1) - a(n-2) with a(1) = 0, a(2) =
 *            140634693
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)0;
    if (n == (idx_t)2)
        return (seq_t)140634693;

    seq_t prevprev = (seq_t)0;
    seq_t prev      = (seq_t)140634693;
    seq_t next;

    for (idx_t i = (idx_t)3; i <= n; ++i) {
        next = ((seq_t)3456296080) * prev - prevprev;
        prevprev = prev;
        prev = next;
    }
    return prev;
}
