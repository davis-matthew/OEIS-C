/*
 * A146512  Numbers congruent to {1, 3} mod 12.
 * https://oeis.org/A146512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*(n-1)+1 if n is odd * a(n) = 6*n‑9 if n is even
 * Range:     u8 n <= 44, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    if (n & 1) {               /* odd index */
        return 6 * (nseq - 1) + 1;
    } else {                   /* even index */
        return 6 * nseq - 9;
    }
}
