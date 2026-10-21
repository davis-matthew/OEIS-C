/*
 * A202401  Number of (n+2)X5 binary arrays avoiding patterns 000 and 010 in
 *          rows and columns.
 * https://oeis.org/A202401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = count of (n+2)×5 binary arrays with no 000 or 010 in any
 *            row or any column. The value is computed by a dynamic
 *            programming over rows, using only addition
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset starts at 1 */

    idx_t m = n + 2;                    /* number of rows */

    /* 1. Determine all legal row patterns (5‑bit strings) that avoid
       000 and 010 inside the row itself. */
    int allowed[32];
    int R = 0;
    for (int mask = 0; mask < 32; ++mask) {
        bool ok = true;
        for (int start = 0; start <= 2 && ok; ++start) {
            int sub = (mask >> start) & 7;   /* 3 consecutive bits */
            if (sub == 0 || sub == 2) {      /* 000 or 010 */
                ok = false;
            }
        }
        if (ok) allowed[R++] = mask;
    }

    /* 2. Build transition lists between states (prevprev, prev).
       A state of id s = a*R + b holds the last two rows a,b.  The
       transition to a new row c is legal iff all columns avoid
       000 and 010 in the triple (a,b,c). */
    int total_states = R * R;
    int *trans_cnt = (int*)calloc(total_states, sizeof(int));
    int **trans   = (int**)malloc(total_states * sizeof(int*));

    /* Count transitions to allocate space */
    for (int a = 0; a < R; ++a) {
        for (int b = 0; b < R; ++b) {
            int s = a * R + b;
            for (int c = 0; c < R; ++c) {
                bool good = true;
                for (int j = 0; j < 5 && good; ++j) {
                    int a_bit = (allowed[a] >> j) & 1;
                    int b_bit = (allowed[b] >> j) & 1;
                    int c_bit = (allowed[c] >> j) & 1;
                    if (a_bit == 0 && b_bit == 0 && c_bit == 0) good = false;
                    else if (a_bit == 0 && b_bit == 1 && c_bit == 0) good = false;
                }
                if (good) ++trans_cnt[s];
            }
        }
    }

    /* Allocate transition lists */
    for (int s = 0; s < total_states; ++s) {
        if (trans_cnt[s] > 0)
            trans[s] = (int*)malloc(trans_cnt[s] * sizeof(int));
        else
            trans[s] = NULL;
    }

    /* Fill transition lists */
    int *temp_cnt = (int*)calloc(total_states, sizeof(int));
    for (int a = 0; a < R; ++a) {
        for (int b = 0; b < R; ++b) {
            int s = a * R + b;
            for (int c = 0; c < R; ++c) {
                bool good = true;
                for (int j = 0; j < 5 && good; ++j) {
                    int a_bit = (allowed[a] >> j) & 1;
                    int b_bit = (allowed[b] >> j) & 1;
                    int c_bit = (allowed[c] >> j) & 1;
                    if (a_bit == 0 && b_bit == 0 && c_bit == 0) good = false;
                    else if (a_bit == 0 && b_bit == 1 && c_bit == 0) good = false;
                }
                if (good) {
                    int idx = temp_cnt[s]++;
                    trans[s][idx] = b * R + c;   /* next state (b,c) */
                }
            }
        }
    }
    free(temp_cnt);

    /* 3. Dynamic programming over the rows.
       dp_cur holds counts for the current last‑two rows. */
    seq_t *dp_cur  = (seq_t*)calloc(total_states, sizeof(seq_t));
    seq_t *dp_next = (seq_t*)calloc(total_states, sizeof(seq_t));

    /* Initialize with all pairs of allowed rows. */
    for (int a = 0; a < R; ++a)
        for (int b = 0; b < R; ++b)
            dp_cur[a * R + b] = 1;

    /* Process rows 3 … m.  Each step extends one more row. */
    for (idx_t step = 3; step <= m; ++step) {
        for (int s = 0; s < total_states; ++s) dp_next[s] = 0;

        for (int s = 0; s < total_states; ++s) {
            seq_t val = dp_cur[s];
            if (!val) continue;
            int tc = trans_cnt[s];
            if (tc == 0) continue;
            int *tlist = trans[s];
            for (int k = 0; k < tc; ++k) {
                int ns = tlist[k];
                dp_next[ns] += val;
            }
        }

        /* swap dp_cur and dp_next */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* 4. Sum all final states to get the total number of arrays. */
    seq_t total = 0;
    for (int s = 0; s < total_states; ++s) total += dp_cur[s];

    /* 5. Clean up */
    free(dp_cur);
    free(dp_next);
    for (int s = 0; s < total_states; ++s) {
        free(trans[s]);
    }
    free(trans);
    free(trans_cnt);

    return total;
}
