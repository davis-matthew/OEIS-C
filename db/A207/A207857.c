/*
 * A207857  Number of nX7 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Recurrence: a(1) = 40; for n≥2 the value is obtained by dynamic programming
   on the 40 admissible 7‑bit rows.  The tables of valid rows, the
   admissible next rows for each ordered pair of rows, and the DP
   transition are all constructed on the first call. */

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* ---------- static data constructed once ---------- */
    static bool initialized = false;
    static int V = 0;                          /* number of admissible rows (40) */
    static int valid_rows[40];                 /* the 7‑bit patterns */
    static int idx_map[128];                   /* map pattern→index, -1 if invalid */
    static int next_count[40][40];             /* admissible next rows count per pair */
    static int next_list[40][40][40];          /* admissible next rows per pair */
    static seq_t dp_cur[40][40];               /* DP for current length      */
    static seq_t dp_next[40][40];              /* DP for next length        */

    /* construct tables once */
    if (!initialized) {
        for (int i = 0; i < 128; ++i) idx_map[i] = -1;
        V = 0;
        /* admissible 7‑bit rows */
        for (int r = 0; r < 128; ++r) {
            bool ok = true;
            for (int pos = 0; pos <= 4 && ok; ++pos) {
                int mask = (r >> pos) & 7;           /* 3 consecutive bits */
                if (mask == 0 || mask == 2) ok = false;   /* 000 or 010 */
            }
            if (ok) {
                valid_rows[V] = r;
                idx_map[r] = V;
                ++V;                  /* V shall be 40 */
            }
        }
        /* pre‑compute admissible transitions */
        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                int cnt = 0;
                for (int k = 0; k < V; ++k) {
                    bool ok = true;
                    for (int col = 0; col < 7 && ok; ++col) {
                        int a = (valid_rows[i] >> col) & 1;
                        int b = (valid_rows[j] >> col) & 1;
                        int c = (valid_rows[k] >> col) & 1;
                        if ((a == 0 && b == 1 && c == 0) ||
                            (a == 1 && b == 0 && c == 1))
                            ok = false;
                    }
                    if (ok) {
                        next_list[i][j][cnt] = k;
                        ++cnt;
                    }
                }
                next_count[i][j] = cnt;
            }
        }
        initialized = true;
    }

    /* ---------- DP for n≥2 (fresh for each call) ---------- */
    /* initialize DP for length 2 (all pairs allowed) */
    for (int i = 0; i < V; ++i)
        for (int j = 0; j < V; ++j)
            dp_cur[i][j] = 1;

    if (n == 1) return (seq_t)V;
    if (n == 2) return (seq_t)V * (seq_t)V;

    /* iterate from length 3 up to n */
    for (idx_t step = 3; step <= n; ++step) {
        /* zero dp_next */
        for (int i = 0; i < V; ++i)
            for (int j = 0; j < V; ++j)
                dp_next[i][j] = 0;

        /* transition */
        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                seq_t val = dp_cur[i][j];
                if (val == 0) continue;
                int cnt = next_count[i][j];
                for (int t = 0; t < cnt; ++t) {
                    int nxt = next_list[i][j][t];
                    dp_next[j][nxt] += val;
                }
            }
        }

        /* move dp_next → dp_cur for next iteration */
        for (int i = 0; i < V; ++i)
            for (int j = 0; j < V; ++j)
                dp_cur[i][j] = dp_next[i][j];
    }

    /* sum all DP states for the final length */
    seq_t total = 0;
    for (int i = 0; i < V; ++i)
        for (int j = 0; j < V; ++j)
            total += dp_cur[i][j];

    return total;
}
