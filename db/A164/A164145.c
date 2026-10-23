/*
 * A164145  Number of binary strings of length n with equal numbers of 001 and
 *          110 substrings.
 * https://oeis.org/A164145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 34, u64 n <= 66, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of binary strings of length n with equal numbers of 001 and 110 substrings.
   DP over the last two bits and the current difference (#001 – #110) is used. */
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Offset and trivial cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;

    idx_t offset = n;                     /* maximum absolute difference needed   */
    size_t sz    = (size_t)(2 * n + 1);    /* diff index range 0 … 2*n              */

    /* Allocate DP layers: 4 states (last 2 bits) × sz diff values */
    seq_t *cur[4];
    seq_t *nxt[4];
    for (int t = 0; t < 4; ++t) {
        cur[t] = (seq_t *)malloc(sz * sizeof(seq_t));
        nxt[t] = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!cur[t] || !nxt[t]) {           /* allocation failed */
            for (int i = 0; i <= t; ++i) {
                free(cur[i]); free(nxt[i]);
            }
            return (seq_t)0;
        }
    }

    /* Initially length = 2: all four pairs, diff = 0 */
    size_t mid = offset;                   /* index for diff = 0 */
    for (int s = 0; s < 4; ++s) {
        memset(cur[s], 0, sz * sizeof(seq_t));
        cur[s][mid] = (seq_t)1;
    }

    /* Iterate lengths 3 … n */
    for (idx_t len = 2; len < n; ++len) {
        /* clear next layer */
        for (int t = 0; t < 4; ++t)
            memset(nxt[t], 0, sz * sizeof(seq_t));

        /* transitions */
        for (int last = 0; last < 4; ++last) {
            int a_bit = last >> 1;
            int b_bit = last & 1;
            seq_t *row = cur[last];
            for (size_t d = 0; d < sz; ++d) {
                seq_t val = row[d];
                if (!val) continue;
                int diff = (int)(d) - (int)offset;
                for (int b = 0; b <= 1; ++b) {
                    int new_last = ((b_bit << 1) | b);
                    int delta = 0;
                    if (a_bit == 0 && b_bit == 0 && b == 1) delta = 1;    /* 001 */
                    else if (a_bit == 1 && b_bit == 1 && b == 0) delta = -1;/* 110 */
                    int new_diff = diff + delta;
                    size_t new_idx = (size_t)(new_diff + (int)offset);
                    nxt[new_last][new_idx] = nxt[new_last][new_idx] + val;
                }
            }
        }

        /* swap current and next */
        for (int t = 0; t < 4; ++t) {
            seq_t *tmp = cur[t];
            cur[t] = nxt[t];
            nxt[t] = tmp;
        }
    }

    /* Sum counts with diff = 0 */
    seq_t ans = 0;
    for (int s = 0; s < 4; ++s)
        ans = ans + cur[s][offset];

    /* free memory */
    for (int t = 0; t < 4; ++t) {
        free(cur[t]);
        free(nxt[t]);
    }

    return ans;
}
