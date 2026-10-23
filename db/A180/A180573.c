/*
 * A180573  Triangle read by rows: T(n,k) is the number of unordered pairs of
 *          vertices at distance k in the sun graph on 2n nodes. The sun graph
 *          on 2n nodes is obtained by attaching n pendant edges to the cycle
 *          graph on n nodes.
 * https://oeis.org/A180573
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the (n‑3)rd element in the triangle of * numbers T(r,k)
 *            = (# unordered pairs of vertices at distance k * in the sun
 *            graph on 2r vertices). Thus a(3)=6, a(4)=6, a(5)=3,
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   cycle_pairs_at_distance
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/cycle_pairs_at_distance.h"

seq_t a(idx_t n) {
    if (n < 3) return (seq_t)0;          /* undefined for n<3, return 0 */

    idx_t r = 3;          /* current row number */
    idx_t start = 3;      /* starting index of this row in the sequence */

    /* Find the row that contains the index n. */
    while (n >= start + (r/2) + 2) {
        start += (r/2) + 2;
        r++;
    }

    idx_t k = n - start + 1;   /* distance within the identified row (1‑based) */

    seq_t result = 0;
    if (k == 1) {
        /* distance 1: cycle-edges plus leaf‑parent edges */
        result = cycle_pairs_at_distance(r, 1) + (seq_t)r;
    } else {
        /* cycle‑cycle pairs at distance k */
        result = cycle_pairs_at_distance(r, k);
        /* leaf–cycle pairs : two for each cycle pair at distance k‑1 */
        seq_t leaf_cycle = cycle_pairs_at_distance(r, k - 1);
        result += leaf_cycle * 2;
        /* leaf–leaf pairs : one for each cycle pair at distance k‑2 */
        if (k >= 3) {
            result += cycle_pairs_at_distance(r, k - 2);
        }
    }
    return result;
}
