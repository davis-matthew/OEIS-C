/*
 * A063736  To get a(n), the pattern of the possible squarefree triple (4n+1,
 *          4n+2, 4n+3) is coded as follows: compute A008966(x) getting one of
 *          {000, 001, 010, 011, 100, 101, 110, 111} and convert from binary.
 * https://oeis.org/A063736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*A008966(4n+1) + 2*A008966(4n+2) + A008966(4n+3)
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A008966(k) = 1 if k is squarefree, 0 otherwise */

#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    /* convert n to seq_t */
    seq_t nseq = (seq_t)n;

    /* compute the three numbers 4n+1, 4n+2, 4n+3 */
    seq_t m1 = 4*nseq + 1;
    seq_t m2 = 4*nseq + 2;
    seq_t m3 = 4*nseq + 3;

    /* evaluate squarefree indicator for each */
    seq_t v1 = is_squarefree(m1) ? 1 : 0;
    seq_t v2 = is_squarefree(m2) ? 1 : 0;
    seq_t v3 = is_squarefree(m3) ? 1 : 0;

    /* combine according to the binary encoding */
    return (4*v1) + (2*v2) + v3;
}
