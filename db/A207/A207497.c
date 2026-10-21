/*
 * A207497  Number of nX5 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts n‑by‑5 binary matrices avoiding the horizontal
 *            patterns 000 and 011 in every row and the vertical patterns 001
 *            and 100 in every column. The DP works on rows: a(1)=13 (the 13
 *            acceptable 5‑bit rows) and a(2)=169 (any pair of acceptable
 *            rows)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* External types */
seq_t a(idx_t n)
{
    /* ---------- static data, computed once ---------- */
    static bool initialized = false;
    static bool allowed[32];
    static int  next_list[32][32][32];
    static int  next_count[32][32];
    static seq_t count_allowed = 0;

    if (!initialized) {
        /* Compute allowed 5‑bit rows (no 000 or 011 horizontally) */
        for (int r = 0; r < 32; ++r) {
            bool ok = true;
            for (int col = 0; col <= 2 && ok; ++col) {
                int bits = (r >> col) & 7;          /* 3 consecutive bits */
                if (bits == 0 || bits == 3)         /* 000 or 011 */
                    ok = false;
            }
            allowed[r] = ok;
            if (ok)
                ++count_allowed;
        }

        /* Build transition lists: for each pair (a,b) list of c so that
           the triple (a,b,c) is vertical‑pattern‑free.        */
        for (int a = 0; a < 32; ++a) {
            for (int b = 0; b < 32; ++b) {
                if (!allowed[a] || !allowed[b]) {
                    next_count[a][b] = 0;
                    continue;
                }
                int cnt = 0;
                for (int c = 0; c < 32; ++c) {
                    if (!allowed[c])
                        continue;
                    bool ok = true;
                    for (int col = 0; col < 5; ++col) {
                        int da = (a >> col) & 1;
                        int db = (b >> col) & 1;
                        int dc = (c >> col) & 1;
                        if ((da == 0 && db == 0 && dc == 1) ||
                            (da == 1 && db == 0 && dc == 0)) {
                            ok = false;
                            break;
                        }
                    }
                    if (ok)
                        next_list[a][b][cnt++] = c;
                }
                next_count[a][b] = cnt;
            }
        }
        initialized = true;
    }

    /* ---------- base cases ---------- */
    if (n == 1)
        return count_allowed;
    if (n == 2)
        return count_allowed * count_allowed;

    /* ---------- DP over rows ---------- */
    static seq_t dp_prev[32][32];
    static seq_t dp_next[32][32];

    /* Initialize dp for the first two rows */
    for (int a = 0; a < 32; ++a)
        for (int b = 0; b < 32; ++b)
            dp_prev[a][b] = 0;
    for (int a = 0; a < 32; ++a)
        for (int b = 0; b < 32; ++b)
            if (allowed[a] && allowed[b])
                dp_prev[a][b] = (seq_t)1;

    idx_t steps = n - 2;
    for (idx_t step = 0; step < steps; ++step) {
        /* Zero dp_next */
        for (int a = 0; a < 32; ++a)
            for (int b = 0; b < 32; ++b)
                dp_next[a][b] = 0;

        /* Transition: for each state (a,b) add all valid next rows c */
        for (int a = 0; a < 32; ++a) {
            for (int b = 0; b < 32; ++b) {
                seq_t val = dp_prev[a][b];
                if (val == 0)
                    continue;
                int cnt = next_count[a][b];
                for (int i = 0; i < cnt; ++i) {
                    int c = next_list[a][b][i];
                    dp_next[b][c] += val;
                }
            }
        }

        /* Copy dp_next to dp_prev for the next iteration */
        for (int a = 0; a < 32; ++a)
            for (int b = 0; b < 32; ++b)
                dp_prev[a][b] = dp_next[a][b];
    }

    /* Sum all states to get total number of matrices */
    seq_t total = 0;
    for (int a = 0; a < 32; ++a) {
        for (int b = 0; b < 32; ++b) {
            if (allowed[a] && allowed[b])
                total += dp_prev[a][b];
        }
    }
    return total;
}
