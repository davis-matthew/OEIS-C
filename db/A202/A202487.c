/*
 * A202487  Number of (n+2)X5 binary arrays avoiding patterns 000 and 010 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2)×5 binary arrays avoiding patterns 000 and
 *            010 in rows, columns and NW‑to‑SE diagonals
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 44,
 *            big any n
 * Helpers:   _row_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/_row_ok.h"

seq_t a(idx_t n) {
    /* static data structures for memoisation */
    static int t = 0;                     /* number of allowed rows */
    static int *rows = NULL;              /* list of allowed rows (5‑bit ints) */
    static int *row_index = NULL;         /* mapping from 5bit value to its index in rows */
    static size_t state_count = 0;        /* t * t */
    static int *adj_len = NULL;
    static int *adj_cap = NULL;
    static int *adj = NULL;               /* adjacency list flattened: state * t + k */

    static seq_t *dp_cur = NULL, *dp_next = NULL;
    static size_t current_rows = 0;       /* rows processed so far (>=2) */
    static seq_t *memo = NULL;           /* memo[n] = a(n) */
    static size_t memo_cap = 0;

    /* initialise data structures once */
    if (!t) {
        /* find allowed rows (5 bits, at most 32 possibilities) */
        int temp_rows[32], cnt = 0;
        for (int r = 0; r < 32; ++r)
            if (_row_ok(r)) temp_rows[cnt++] = r;

        t = cnt;
        rows = (int*)malloc(t * sizeof(int));
        for (int i = 0; i < t; ++i) rows[i] = temp_rows[i];

        row_index = (int*)malloc(32 * sizeof(int));
        for (int i = 0; i < 32; ++i) row_index[i] = -1;
        for (int i = 0; i < t; ++i) row_index[rows[i]] = i;

        state_count = (size_t)t * (size_t)t;

        /* adjacency lists: each state can transition to at most t next rows */
        adj_cap = (int*)calloc(state_count, sizeof(int));
        adj_len = (int*)calloc(state_count, sizeof(int));
        adj = (int*)malloc(state_count * t * sizeof(int));

        /* build transitions */
        for (int i = 0; i < t; ++i) {          /* prevprev row */
            int r1 = rows[i];
            for (int j = 0; j < t; ++j) {      /* prev row */
                int r2 = rows[j];
                size_t st = (size_t)i * t + j;
                for (int k = 0; k < t; ++k) {  /* candidate new row */
                    int r3 = rows[k];
                    int ok = 1;

                    /* vertical triple check for each column */
                    for (int col = 0; col < 5 && ok; ++col) {
                        int a = (r1 >> col) & 1;
                        int b = (r2 >> col) & 1;
                        int c = (r3 >> col) & 1;
                        if (a == 0 && b == 0 && c == 0) ok = 0;
                        else if (a == 0 && b == 1 && c == 0) ok = 0;
                    }

                    /* diagonal NW‑to‑SE triple check */
                    for (int col = 2; col < 5 && ok; ++col) {
                        int a = (r1 >> (col - 2)) & 1;
                        int b = (r2 >> (col - 1)) & 1;
                        int c = (r3 >> col) & 1;
                        if (a == 0 && b == 0 && c == 0) ok = 0;
                        else if (a == 0 && b == 1 && c == 0) ok = 0;
                    }

                    if (ok) {
                        int next = (int)(j * t + k);
                        int idx = adj_len[st];
                        adj[st * t + idx] = next;
                        adj_len[st] = idx + 1;
                    }
                }
            }
        }

        /* allocate DP arrays */
#ifdef __cplusplus
        dp_cur = new seq_t[state_count];
        dp_next = new seq_t[state_count];
#else
        dp_cur = (seq_t*)malloc(state_count * sizeof(seq_t));
        dp_next = (seq_t*)malloc(state_count * sizeof(seq_t));
#endif
        /* initialise dp_cur with all pairs (first 2 rows) */
        for (size_t s = 0; s < state_count; ++s)
            dp_cur[s] = 1;
        current_rows = 2;                 /* two rows processed */

        /* memo array: index n => a(n). allocate capacity for up to 20000 terms. */
        memo_cap = 20001;
        memo = (seq_t*)calloc(memo_cap, sizeof(seq_t));
    }

    /* compute up to required number of rows */
    size_t target_rows = (size_t)n + 2;    /* total rows needed */
    while (current_rows < target_rows) {
        /* zero dp_next */
        for (size_t i = 0; i < state_count; ++i) dp_next[i] = 0;

        /* transition */
        for (size_t st = 0; st < state_count; ++st) if (dp_cur[st] != 0) {
            seq_t val = dp_cur[st];
            int len = adj_len[st];
            int *list = &adj[st * t];
            for (int idx = 0; idx < len; ++idx)
                dp_next[list[idx]] += val;
        }

        /* swap dp arrays */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
        current_rows++;

        /* store sum into memo[n] where n = current_rows - 2 */
        seq_t total = 0;
        for (size_t s = 0; s < state_count; ++s) total += dp_cur[s];
        size_t idx_n = current_rows - 2;
        if (idx_n < memo_cap)
            memo[idx_n] = total;
    }

    return memo[n];
}
