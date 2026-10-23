/*
 * A047120  Array T read by diagonals: T(h,k)=number of paths consisting of
 *          steps from (0,0) to (h,k) such that each step has length 1
 *          directed up or right and touches the line y=x/4 only at lattice
 *          points.
 * https://oeis.org/A047120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) counts monotone lattice paths from (0,0) to (h,k) with
 *            steps (1,0) or (0,1) that touch the line y = x/4 only at lattice
 *            points. The index n is mapped to (h,k) by antidiagonal ordering
 *            (OEIS style). The value is computed by a simple O(h·k) dynamic
 *            programming recurrence
 * Range:     u8 n <= 70, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   _antidiag_coordinates
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/_antidiag_coordinates.h"

#define MAXD 200   /* sufficient for n up to about 10000 (h,k ≤ 141) */

seq_t a(idx_t n)
{
    idx_t h, k;
    _antidiag_coordinates(n, &h, &k);
    if (h >= MAXD || k >= MAXD) {
        /* Index out of pre‑allocated table; fall back to dynamic array */
        size_t width = (size_t)(k + 1);
        seq_t *prev = (seq_t *)calloc(width, sizeof(seq_t));
        seq_t *curr = (seq_t *)calloc(width, sizeof(seq_t));
        if (!prev || !curr) {
            free(prev);
            free(curr);
            return (seq_t)0;
        }
        for (idx_t i = 0; i <= h; ++i) {
            for (idx_t j = 0; j <= k; ++j) {
                if (i == 0 && j == 0) {
                    curr[j] = (seq_t)1;
                    continue;
                }
                seq_t val = (seq_t)0;
                if (i > 0) val += prev[j];
                if (j > 0) {
                    int allow = 1;
                    if (i % 4 != 0 && (i / 4) == j - 1) allow = 0;
                    if (allow) val += curr[j - 1];
                }
                curr[j] = val;
            }
            seq_t *tmp = prev; prev = curr; curr = tmp;
        }
        seq_t result = prev[k];
        free(prev);
        free(curr);
        return result;
    }

    static seq_t dp_prev[MAXD];
    static seq_t dp_curr[MAXD];

    /* clear only the used portion */
    for (idx_t j = 0; j <= k; ++j) dp_prev[j] = dp_curr[j] = (seq_t)0;

    for (idx_t i = 0; i <= h; ++i) {
        for (idx_t j = 0; j <= k; ++j) {
            if (i == 0 && j == 0) {
                dp_curr[0] = (seq_t)1;
                continue;
            }
            seq_t val = (seq_t)0;
            if (i > 0) val += dp_prev[j];
            if (j > 0) {
                int allow = 1;
                if (i % 4 != 0 && (i / 4) == j - 1) allow = 0;
                if (allow) val += dp_curr[j - 1];
            }
            dp_curr[j] = val;
        }
        /* copy current row to previous for next iteration */
        for (idx_t j = 0; j <= k; ++j) dp_prev[j] = dp_curr[j];
    }
    return dp_prev[k];
}
