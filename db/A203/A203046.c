/*
 * A203046  Number of (n+3)X9 binary arrays with no more than one of any
 *          consecutive four bits set in any row or column.
 * https://oeis.org/A203046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 14, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A203046 : Number of (n+3)×9 binary arrays with no more than one
    of any consecutive four bits set in any row or column.  The array
    is counted by a transfer‑matrix DP over rows.  Each row is a
    9‑bit pattern with no two 1‑bits within distance ≤3.  The set of
    such patterns has size R=26.  A state consists of the last three
    rows (ordered) that must be pairwise disjoint.  Transitions
    add a new row that is disjoint from the union of the three rows
    in the current state.  The DP iterates over N=n+3 rows.  The sum
    of all state counts after the last row yields a(n).  This method
    runs in O(R³·N)~2·10⁷ operations for n≈10⁴.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* assume seq_t and idx_t are defined in the build environment */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* number of rows in the matrix */
    const idx_t N = n + 3;          /* N <= 10003 for n up to 10000 */

    /* ---------- precomputation run once ---------- */
    static bool init = false;
    static int R;                   /* number of allowed row patterns */
    static uint16_t pat[32];        /* pattern masks (max 27 bits needed) */
    static int pat_idx[512];        /* map mask -> index, -1 if not allowed */
    static int *state_i, *state_j, *state_k; /* state decomposition */
    static int *state_map;          /* map (i,j,k) -> state id, -1 if invalid */
    static int S;                   /* number of valid states */
    static int (*next_states)[26];   /* adjacency lists, up to 26 entries per state */
    static int *next_cnt;           /* number of outgoing transitions per state */

    if (!init)
    {
        /* 1. generate all allowed row patterns (26 of them) */
        R = 0;
        for (int mask = 0; mask < 512; ++mask) {
            if ( (mask & (mask >> 1))  != 0) continue;
            if ( (mask & (mask >> 2))  != 0) continue;
            if ( (mask & (mask >> 3))  != 0) continue;
            pat[R++] = (uint16_t)mask;
        }

        /* map mask to index */
        for (int i = 0; i < 512; ++i) pat_idx[i] = -1;
        for (int i = 0; i < R; ++i) pat_idx[pat[i]] = i;

        /* 2. create states: ordered triples of pairwise disjoint patterns */
        int max_states = R * R * R;   /* upper bound, 26^3=17576 */
        state_map = (int *)malloc(max_states * sizeof(int));
        for (int i = 0; i < max_states; ++i) state_map[i] = -1;
        state_i = (int *)malloc(max_states * sizeof(int));
        state_j = (int *)malloc(max_states * sizeof(int));
        state_k = (int *)malloc(max_states * sizeof(int));
        S = 0;
        for (int a = 0; a < R; ++a) {
            for (int b = 0; b < R; ++b) {
                if ( (pat[a] & pat[b]) != 0) continue;
                for (int c = 0; c < R; ++c) {
                    if ( (pat[a] & pat[c]) != 0 ) continue;
                    if ( (pat[b] & pat[c]) != 0 ) continue;
                    int idx = (a*R + b)*R + c;
                    state_map[idx] = S;
                    state_i[S] = a;
                    state_j[S] = b;
                    state_k[S] = c;
                    ++S;
                }
            }
        }

        /* 3. adjacency lists */
        next_states = (int (*)[26])malloc(S * 26 * sizeof(int));
        next_cnt = (int *)calloc(S, sizeof(int));
        for (int s = 0; s < S; ++s) {
            int a = state_i[s], b = state_j[s], c = state_k[s];
            uint16_t used = pat[a] | pat[b] | pat[c];
            for (int t = 0; t < R; ++t) {
                if ( (pat[t] & used) == 0 ) {
                    int idx2 = (b*R + c)*R + t;
                    int ns = state_map[idx2];
                    if (ns >= 0) {
                        int pos = next_cnt[s]++;
                        next_states[s][pos] = ns;
                    }
                }
            }
        }

        init = true;
    }

    /* ---------- DP over rows ---------- */
    seq_t *dp_cur = (seq_t *)calloc(S, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(S, sizeof(seq_t));

    /* initial states: any triple of pairwise disjoint rows */
    for (int s = 0; s < S; ++s) dp_cur[s] = 1;

    for (idx_t step = 4; step <= N; ++step) {
        /* zero dp_next */
        for (int i = 0; i < S; ++i) dp_next[i] = 0;
        /* transitions */
        for (int s = 0; s < S; ++s) {
            seq_t val = dp_cur[s];
            if (val == 0) continue;
            int cnt = next_cnt[s];
            for (int j = 0; j < cnt; ++j) {
                int ns = next_states[s][j];
                dp_next[ns] += val;
            }
        }
        /* swap */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* sum all ending states */
    seq_t ans = 0;
    for (int s = 0; s < S; ++s) ans += dp_cur[s];

    /* clean up */
    free(dp_cur);
    free(dp_next);

    return ans;
}
