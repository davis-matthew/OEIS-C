/*
 * A211699  Number of nonnegative integer arrays of length n+2*7-2 with new
 *          values introduced in order 0 upwards and every value appearing
 *          only in runs of at least 7.
 * https://oeis.org/A211699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 35, u32 n <= 66, u64 n <= 120,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A211699: Number of nonnegative integer arrays of length n+12
   where new values appear in order 0 upwards and every value occurs
   in contiguous runs of length at least 7.  The numbers are obtained
   by a dynamic programme over block lengths and the number of
   distinct values.  */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    const idx_t w = 7;                /* minimum run length          */
    idx_t L = n + 12;                 /* total array length          */
    idx_t max_d = L / w;              /* maximum possible distinct values */

    const size_t ringSize = w + 1;                    /* store last w+1 cumulative sums */
    const size_t dim = (size_t)max_d + 1;             /* indices 0 .. max_d            */

    /* ------- dynamic memory allocation ------------------------------- */
    seq_t *cum   = (seq_t*)calloc(dim, sizeof(seq_t));
    seq_t **ring = (seq_t**)malloc(ringSize * sizeof(seq_t*));
    if (!cum || !ring) return (seq_t)0;
    for (size_t i = 0; i < ringSize; ++i) {
        ring[i] = (seq_t*)calloc(dim, sizeof(seq_t));
        if (!ring[i]) return (seq_t)0;
    }
    seq_t *dp_cur = (seq_t*)calloc(dim, sizeof(seq_t));
    if (!dp_cur) return (seq_t)0;

    /* ------- base case: empty array --------------------------------- */
    cum[0] = (seq_t)1;
    ring[0][0] = (seq_t)1;            /* cumulative sum at index 0 for d=0 */

    /* ------- DP ------------------------------------------------------- */
    for (idx_t l = 1; l <= L; ++l) {
        /* clear current dp for this length */
        for (size_t d = 0; d < dim; ++d) dp_cur[d] = 0;

        if (l >= w) {
            size_t winIdx = (size_t)((l - w) % ringSize);   /* point to prefix sums */

            for (size_t d = 1; d < dim; ++d) {
                seq_t pref_new   = ring[winIdx][d-1];   /* sum_{t<=l-w} dp[t][d-1] */
                seq_t pref_reuse = (d > 1) ? ring[winIdx][d] : 0; /* sum_{t<=l-w} dp[t][d] */
                dp_cur[d] = pref_new;                     /* new distinct value */
                if (d > 1) dp_cur[d] += (seq_t)(d-1) * pref_reuse; /* reuse existing one */
            }
        }

        /* update cumulative sums and ring buffer for future accesses */
        size_t modIdx = (size_t)(l % ringSize);
        for (size_t d = 0; d < dim; ++d) {
            cum[d] += dp_cur[d];
            ring[modIdx][d] = cum[d];
        }
    }

    /* ------- sum over all possible numbers of distinct values -------- */
    seq_t answer = 0;
    for (size_t d = 1; d < dim; ++d) answer += dp_cur[d];

    /* ------- free allocated memory ----------------------------------- */
    free(dp_cur);
    for (size_t i = 0; i < ringSize; ++i) free(ring[i]);
    free(ring);
    free(cum);

    return answer;
}
