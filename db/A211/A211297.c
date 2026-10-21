/*
 * A211297  a(n) = number of n-lettered words in the alphabet {1, 2, 3} with
 *          as many occurrences of the substring (consecutive subword) [1, 2,
 *          1] as of [1, 3, 1].
 * https://oeis.org/A211297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length‑n words over {1,2,3} with occurrences of
 *            the substring 1,2,1 equal to occurrences of 1,3,1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Pre‑computed mapping from a pair (x,y) to state index 0..8. */
static const int pair_to_index[4][4] = {
    { -1, -1, -1, -1 },        /* 0 is unused */
    { -1,  0,  1,  2 },        /* 1→x */
    { -1,  3,  4,  5 },        /* 2→x */
    { -1,  6,  7,  8 }         /* 3→x */
};

/* For every state s = pair(x,y) and for each new letter l=1..3:
   next_state[s][l‑1] = index of the state (y,l),
   delta_shift[s][l‑1] = +1 if (x,y,l) == (1,2,1),
                         -1 if (x,y,l) == (1,3,1),
                          0 otherwise. */
static const int next_state[9][3] = {
    /* x=1,y=1 */
    { pair_to_index[1][1], pair_to_index[1][2], pair_to_index[1][3] },
    /* x=1,y=2 */
    { pair_to_index[2][1], pair_to_index[2][2], pair_to_index[2][3] },
    /* x=1,y=3 */
    { pair_to_index[3][1], pair_to_index[3][2], pair_to_index[3][3] },
    /* x=2,y=1 */
    { pair_to_index[1][1], pair_to_index[1][2], pair_to_index[1][3] },
    /* x=2,y=2 */
    { pair_to_index[2][1], pair_to_index[2][2], pair_to_index[2][3] },
    /* x=2,y=3 */
    { pair_to_index[3][1], pair_to_index[3][2], pair_to_index[3][3] },
    /* x=3,y=1 */
    { pair_to_index[1][1], pair_to_index[1][2], pair_to_index[1][3] },
    /* x=3,y=2 */
    { pair_to_index[2][1], pair_to_index[2][2], pair_to_index[2][3] },
    /* x=3,y=3 */
    { pair_to_index[3][1], pair_to_index[3][2], pair_to_index[3][3] }
};

static const signed char delta_shift[9][3] = {
    /* x=1,y=1 */
    { 0, 0, 0 },
    /* x=1,y=2 */
    { 1, 0, 0 },
    /* x=1,y=3 */
    { -1, 0, 0 },
    /* x=2,y=1 */
    { 0, 0, 0 },
    /* x=2,y=2 */
    { 0, 0, 0 },
    /* x=2,y=3 */
    { 0, 0, 0 },
    /* x=3,y=1 */
    { 0, 0, 0 },
    /* x=3,y=2 */
    { 0, 0, 0 },
    /* x=3,y=3 */
    { 0, 0, 0 }
};

seq_t a(idx_t n) {
    /* Handle base cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)9;

    /* Offset for delta index */
    idx_t offset = n;
    size_t size = (size_t)(2 * offset + 1);

    /* Allocate two DP layers */
    seq_t *curr[9];
    seq_t *next[9];
    for (int s = 0; s < 9; ++s) {
        curr[s] = (seq_t *)calloc(size, sizeof(seq_t));
        next[s] = (seq_t *)calloc(size, sizeof(seq_t));
    }

    /* Initialize DP for words of length 2 */
    for (int s = 0; s < 9; ++s) {
        curr[s][offset] = (seq_t)1;
    }

    /* Track active range for each state in curr */
    int curr_min[9];
    int curr_max[9];
    for (int s = 0; s < 9; ++s) {
        curr_min[s] = curr_max[s] = (int)offset;
    }

    /* Main DP loop: extend to length n */
    for (idx_t len = 3; len <= n; ++len) {
        /* Reset next bounds */
        int next_min[9];
        int next_max[9];
        for (int s = 0; s < 9; ++s) {
            next_min[s] = next_max[s] = -1;
        }

        /* Transition from curr to next */
        for (int s = 0; s < 9; ++s) {
            int minIdx = curr_min[s];
            int maxIdx = curr_max[s];
            for (int d = minIdx; d <= maxIdx; ++d) {
                seq_t val = curr[s][d];
                if (val == (seq_t)0) continue;

                /* Convert index to delta value */
                int delta = d - (int)offset;

                for (int l = 1; l <= 3; ++l) {
                    int ns = next_state[s][l - 1];
                    int shift = delta_shift[s][l - 1];
                    int ndelta = delta + shift;
                    int idxNew = (int)offset + ndelta;

                    next[ns][idxNew] += val;

                    if (next_min[ns] == -1) {
                        next_min[ns] = next_max[ns] = idxNew;
                    } else {
                        if (idxNew < next_min[ns]) next_min[ns] = idxNew;
                        if (idxNew > next_max[ns]) next_max[ns] = idxNew;
                    }
                }
            }
        }

        /* Prepare for next iteration: swap layers and clear old curr */
        for (int s = 0; s < 9; ++s) {
            int old_min = curr_min[s];
            int old_max = curr_max[s];
            /* Zero the old cur array in its used range */
            if (old_min <= old_max) {
                for (int k = old_min; k <= old_max; ++k) {
                    curr[s][k] = (seq_t)0;
                }
            }
            /* Swap pointers */
            seq_t *tmp = curr[s];
            curr[s] = next[s];
            next[s] = tmp;
            /* Update bounds */
            curr_min[s] = next_min[s];
            curr_max[s] = next_max[s];
            next_min[s] = next_max[s] = -1;
        }
    }

    /* Sum over all states for delta = 0 */
    seq_t ans = (seq_t)0;
    for (int s = 0; s < 9; ++s) {
        ans += curr[s][offset];
    }

    /* Free allocated memory */
    for (int s = 0; s < 9; ++s) {
        free(curr[s]);
        free(next[s]);
    }

    return ans;
}
