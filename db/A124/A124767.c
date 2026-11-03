/*
 * A124767  Number of level runs for compositions in standard order.
 * https://oeis.org/A124767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 121 terms through n = 8185 (2026-11-01)
 * Helpers:   ilog2_idx
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
#include <stdlib.h>
#include "../utils/ilog2_idx.h"

/* Recursive helper: computes number of runs in the composition of given sum.
   sum   : total sum of the composition
   idx   : 0‑based index within the list of compositions of this sum
   runs  : output number of runs
   first : output first part of the composition (needed for counting runs)
*/
static void _runs_first(idx_t sum, idx_t idx, seq_t *runs, int *first) {
    if (sum == 0) {
        *runs = 0;            /* empty composition has no runs */
        *first = 0;
        return;
    }
    for (int k = (int)sum; k >= 1; --k) {
        idx_t rest_sum = sum - k;
        idx_t blocksize = (rest_sum == 0) ? 1 : ((idx_t)1 << (rest_sum - 1));
        if (idx < blocksize) {
            *first = k;
            if (rest_sum == 0) {
                *runs = 1;
            } else {
                seq_t runs_rest;
                int first_rest;
                _runs_first(rest_sum, idx, &runs_rest, &first_rest);
                *runs = runs_rest + ((k != first_rest) ? (seq_t)1 : (seq_t)0);
            }
            return;
        }
        idx -= blocksize;     /* skip this block */
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* a(0) = 0 */

    /* Determine the sum S such that compositions of sums < S are < n
       and sum S compositions cover n.  For n ≥ 1,  S = floor_log2(n) + 1   */
    idx_t S = ilog2_idx(n) + 1;

    /* 2^(S-1) is the first index of compositions of sum S */
    idx_t start = ((idx_t)1 << (S - 1));
    idx_t idx_in_sum = n - start;             /* 0‑based index within sum S */

    seq_t runs;
    int first_part;                           /* not used after final call */
    _runs_first(S, idx_in_sum, &runs, &first_part);
    return runs;
}
