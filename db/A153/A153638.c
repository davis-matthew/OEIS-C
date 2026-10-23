/*
 * A153638  Odiousness of triangular numbers.
 * https://oeis.org/A153638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if popcount(n*(n+1)/2) is odd, else 0
 * Range:     u8 n <= 22, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* compute the n-th triangular number */
    seq_t tri = (seq_t)n * ((seq_t)n + 1) / 2;

    /* determine parity of the number of 1-bits */
    bool odd = false;
    seq_t t = tri;
    while (t) {
        odd ^= (bool)(t & 1);
        t >>= 1;
    }
    return odd ? (seq_t)1 : (seq_t)0;
}
