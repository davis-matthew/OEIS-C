/*
 * A124767  Number of level runs for compositions in standard order.
 * https://oeis.org/A124767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   _runs_first, floor_log2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A124767: Number of level runs for compositions in standard order.
   a(0) = 0.  For n > 0,
   let Cn be the n‑th composition in standard order (order = reverse lexicographic
   among all compositions of the same sum, sums listed in increasing size).
   Then a(n) = number of runs of equal parts in Cn.
   This implementation builds the composition implicitly and counts runs.
*/

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_runs_first.h"
#include "../utils/floor_log2.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* a(0) = 0 */

    /* Determine the sum S such that compositions of sums < S are < n
       and sum S compositions cover n.  For n ≥ 1,  S = floor_log2(n) + 1   */
    idx_t S = floor_log2(n) + 1;

    /* 2^(S-1) is the first index of compositions of sum S */
    idx_t start = ((idx_t)1 << (S - 1));
    idx_t idx_in_sum = n - start;             /* 0‑based index within sum S */

    seq_t runs;
    int first_part;                           /* not used after final call */
    _runs_first(S, idx_in_sum, &runs, &first_part);
    return runs;
}
