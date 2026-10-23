/*
 * A032192  Number of necklaces with 7 black beads and n-7 white beads.
 * https://oeis.org/A032192
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(binomial(n,7)/n) (equivalently a(n) = (binomial(n,7)
 *            + n - 1) / n)
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n < 7) return 0;         /* undefined for offset, but safe */
    seq_t c = binomial(n, 7);     /* C(n,7) as seq_t */
    seq_t nseq = (seq_t)n;
    return (c + nseq - 1) / nseq; /* ceiling division */
}
