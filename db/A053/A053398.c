/*
 * A053398  Nim-values from game of Kopper's Nim.
 * https://oeis.org/A053398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   a(n) = position of the least significant zero bit of
 *            (⌊(x-1)|(y-1)⌋) where (x,y) are obtained from the n-th entry in
 *            the antidiagonal enumeration of all pairs of positive integers
 *            (x,y). The enumeration orders pairs first by the sum s=x+y, then
 *            by increasing x. The value returned is 0 for the fir...
 * Range:     u8 n <= 7875, u16 n <= 7875, u32 n <= 7875, u64 n <= 7875,
 *            u128 n <= 7875, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the diagonal sum s such that n lies in the diagonal of pairs with
       sum s = x + y.  The number of pairs with sum up to s is (s-1)*s/2. */
    idx_t s = 2;
    while (((unsigned __int128)s * (s - 1) / 2) < n)
        ++s;

    /* Number of pairs before this diagonal */
    idx_t prev = (s - 1) * (s - 2) / 2;

    /* Determine x and y */
    idx_t x = n - prev;          /* 1-based index within the diagonal */
    idx_t y = s - x;             /* because x + y = s */

    /* Compute (x-1)|(y-1) and find the position of its first zero bit. */
    seq_t val = (seq_t)(x - 1) | (seq_t)(y - 1);

    seq_t pos = 0;
    while (val & (seq_t)1) {
        val >>= 1;
        ++pos;
    }
    return pos;
}
