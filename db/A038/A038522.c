/*
 * A038522  On a (2n+1) X (2n+1) board, let m(i) be the number of squares that
 *          are i knight's moves from center; sequence gives max m(i) for i >=
 *          0.
 * https://oeis.org/A038522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, walk, nice
 * Formula:   a(n) = 28*floor(n/2) - 20 for n >= 10 a(0..9) are given by the
 *            following table
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* first 10 terms as given in the OEIS entry */
    static const seq_t init[10] = {
        1, 1, 8, 20, 32, 52, 68, 76, 96, 96
    };

    if (n < 10) {
        return init[(size_t)n];
    }

    /* compute a(n) using only seq_t arithmetic */
    seq_t sn   = (seq_t)n;          /* convert index to seq_t first */
    seq_t half = sn >> 1;           /* floor(n/2) */
    seq_t res  = (seq_t)28 * half - (seq_t)20;
    return res;
}
