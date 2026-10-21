/*
 * A207943  Number of 7Xn 0..1 arrays avoiding 0 0 1 and 1 0 0 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Generating function not provided – implement by transfer‑matrix
 *            DP
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Seq. A207943: Number of 7×n 0..1 arrays avoiding 0 0 1 and 1 0 0 horizontally
   and 0 0 1 and 1 1 0 vertically.  The DP keeps the last two columns
   (each a 7‑bit pattern satisfying the vertical restriction) and
   expands column by column using the horizontal constraints.  The
   number of valid columns is 14, so the state space is 14×14=196.
   This approach works in O(n·state‑edges) and uses only
   unsigned arithmetic (seq_t). */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int vertical_ok(uint8_t col)
{
    /* check for patterns 001 and 110 in a 7‑bit column */
    for (int i = 0; i <= 4; ++i) {
        uint8_t pat = (col >> i) & 0x7;      /* 3 bits */
        if (pat == 0b001 || pat == 0b110)
            return 0;
    }
    return 1;
}

seq_t a(idx_t n)
{
    /* ---------- static data (built once) ---------- */
    static int init_flag = 0;

    /* columns satisfying vertical restriction */
    static uint8_t *valid_cols = NULL;   /* length M  */
    static int *col_to_idx = NULL;       /* 128 entries, -1 if invalid   */
    static int M = 0;                    /* number of valid columns      */
    static int S = 0;                    /* number of states = M*M        */
    static int *adj_cnt = NULL;          /* per state adjacency count    */
    static int **adj_list = NULL;        /* per state adjacency list     */

    /* DP caches */
    static seq_t *dp_prev = NULL;        /* counts for current length     */
    static seq_t *dp_next = NULL;        /* temporary for next length     */
    static idx_t cache_len = 0;          /* highest n computed so far     */

    /* results cache */
    static seq_t *results = NULL;        /* 1‑based indices, results[n]  */
    static idx_t results_cap = 0;        /* allocated size for results    */

    /* ---------- initialization ---------- */
    if (!init_flag) {
        /* 1. Build list of valid columns */
        uint8_t all_cols[128];
        int idx = 0;
        for (int col = 0; col < 128; ++col) {
            if (vertical_ok((uint8_t)col)) {
                all_cols[idx++] = (uint8_t)col;
            }
        }
        M = idx;
        S = M * M;

        valid_cols = (uint8_t *)malloc(M * sizeof(uint8_t));
        memcpy(valid_cols, all_cols, M * sizeof(uint8_t));

        col_to_idx = (int *)malloc(128 * sizeof(int));
        for (int i = 0; i < 128; ++i) col_to_idx[i] = -1;
        for (int i = 0; i < M; ++i) col_to_idx[valid_cols[i]] = i;

        /* 2. Build adjacency lists */
        adj_cnt = (int *)calloc(S, sizeof(int));
        adj_list = (int **)malloc(S * sizeof(int *));

        for (int a = 0; a < M; ++a) {
            int sa = valid_cols[a];
            for (int b = 0; b < M; ++b) {
                int sb = valid_cols[b];
                int state = a * M + b;

                uint8_t must_zero = 0;
                uint8_t must_one  = 0;
                for (int r = 0; r < 7; ++r) {
                    int bit_a = (sa >> r) & 1;
                    int bit_b = (sb >> r) & 1;
                    if (bit_a == 0 && bit_b == 0)
                        must_zero |= (1 << r);      /* column bit must be 0 */
                    else if (bit_a == 1 && bit_b == 0)
                        must_one  |= (1 << r);      /* column bit must be 1 */
                }

                /* count neighbors */
                int cnt = 0;
                for (int c = 0; c < M; ++c) {
                    int sc = valid_cols[c];
                    if ( ((sc & must_zero) == 0) &&
                         ((sc & must_one)  == must_one)) {
                        ++cnt;
                    }
                }
                adj_cnt[state] = cnt;
            }
        }

        /* allocate and fill lists */
        for (int a = 0; a < M; ++a) {
            for (int b = 0; b < M; ++b) {
                int state = a * M + b;
                int cnt   = adj_cnt[state];
                int *list = (int *)malloc(cnt * sizeof(int));
                int pos = 0;
                uint8_t sa = valid_cols[a];
                uint8_t sb = valid_cols[b];

                uint8_t must_zero = 0;
                uint8_t must_one  = 0;
                for (int r = 0; r < 7; ++r) {
                    int bit_a = (sa >> r) & 1;
                    int bit_b = (sb >> r) & 1;
                    if (bit_a == 0 && bit_b == 0)
                        must_zero |= (1 << r);
                    else if (bit_a == 1 && bit_b == 0)
                        must_one  |= (1 << r);
                }

                for (int c = 0; c < M; ++c) {
                    int sc = valid_cols[c];
                    if ( ((sc & must_zero) == 0) &&
                         ((sc & must_one)  == must_one)) {
                        list[pos++] = b * M + c;   /* state for (b,c) */
                    }
                }
                adj_list[state] = list;
            }
        }

        /* 3. Allocate DP arrays */
        dp_prev = (seq_t *)malloc(S * sizeof(seq_t));
        dp_next = (seq_t *)malloc(S * sizeof(seq_t));

        /* 4. Result cache */
        results_cap = 3; /* indices 0..2 used initially */
        results = (seq_t *)malloc(results_cap * sizeof(seq_t));
        results[0] = 0;  /* unused */

        /* base cases */
        results[1] = (seq_t)M;                     /* a(1) = number of columns */
        results[2] = (seq_t)M * (seq_t)M;           /* a(2) = M^2 */

        /* initialize DP for length 2: all states have value 1 */
        for (int i = 0; i < S; ++i)
            dp_prev[i] = (seq_t)1;

        cache_len = 2; /* computed up to n=2 */
        init_flag = 1;
    }

    /* ---------- use cache or compute up to n ---------- */
    if (n <= cache_len) {
        return results[n];
    }

    /* Ensure results array large enough */
    if (n >= results_cap) {
        idx_t new_cap = n + 1;
        results = (seq_t *)realloc(results, new_cap * sizeof(seq_t));
        results_cap = new_cap;
    }

    /* Extend DP from cache_len+1 up to n */
    for (idx_t len = cache_len + 1; len <= n; ++len) {
        /* zero dp_next */
        for (int i = 0; i < S; ++i)
            dp_next[i] = 0;

        /* DP transition */
        for (int s = 0; s < S; ++s) {
            seq_t v = dp_prev[s];
            if (!v) continue;
            int cnt = adj_cnt[s];
            int *list = adj_list[s];
            for (int k = 0; k < cnt; ++k) {
                dp_next[list[k]] += v;
            }
        }

        /* compute a(len) = sum over dp_next */
        seq_t sum = 0;
        for (int i = 0; i < S; ++i) {
            sum += dp_next[i];
        }
        results[len] = sum;

        /* swap dp arrays for next iteration */
        seq_t *tmp = dp_prev;
        dp_prev = dp_next;
        dp_next = tmp;
    }

    cache_len = n;
    return results[n];
}
