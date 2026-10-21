/*
 * A207753  Number of 4 X n 0..1 arrays avoiding 0 0 1 and 0 1 1 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 78, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=9, a(2)=81; for n≥3 it counts 4×n binary arrays avoiding
   the patterns 0 0 1 and 0 1 1 horizontally, and 0 0 1 and 1 0 0 vertically.
   Computation is done by dynamic programming over pairs of allowed columns. */
seq_t a(idx_t n) {
    /* Pre‑computation of allowed columns and transition lists, done once. */
    static int init = 0;
    static unsigned char allowed[16];
    static int allowed_count;
    static unsigned char nextz[256][16];
    static unsigned char nextcnt[256];
    if (!init) {
        /* Determine which 4‑bit columns have no forbidden vertical pattern. */
        for (int col = 0; col < 16; ++col) {
            int a = (col >> 0) & 1;
            int b = (col >> 1) & 1;
            int c = (col >> 2) & 1;
            int d = (col >> 3) & 1;
            int ok = 1;
            if ((a == 0 && b == 0 && c == 1) || (a == 1 && b == 0 && c == 0))
                ok = 0;
            if ((b == 0 && c == 0 && d == 1) || (b == 1 && c == 0 && d == 0))
                ok = 0;
            allowed[col] = ok;
        }
        allowed_count = 0;
        for (int col = 0; col < 16; ++col)
            if (allowed[col]) ++allowed_count;

        /* For each pair (x,y) pre‑compute the list of z that form a valid
           horizontal triple with x and y. */
        for (int x = 0; x < 16; ++x) {
            for (int y = 0; y < 16; ++y) {
                int idx = (x << 4) | y;   /* x*16 + y */
                unsigned char cnt = 0;
                if (!allowed[x] || !allowed[y]) {
                    nextcnt[idx] = 0;
                    continue;
                }
                for (int z = 0; z < 16; ++z) {
                    if (!allowed[z]) continue;
                    int ok = 1;
                    for (int r = 0; r < 4; ++r) {
                        int bx = (x >> r) & 1;
                        int by = (y >> r) & 1;
                        int bz = (z >> r) & 1;
                        if ((bx == 0 && by == 0 && bz == 1) ||
                            (bx == 0 && by == 1 && bz == 1)) {
                            ok = 0;
                            break;
                        }
                    }
                    if (ok) {
                        nextz[idx][cnt++] = (unsigned char)z;
                    }
                }
                nextcnt[idx] = cnt;
            }
        }
        init = 1;
    }

    /* Handle trivial small n. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)allowed_count;
    if (n == 2) return (seq_t)allowed_count * (seq_t)allowed_count;

    /* DP over pairs of columns. */
    static seq_t dp[256];
    static seq_t dp_next[256];
    /* Initialise for length 2 (any allowed pair). */
    for (int i = 0; i < 256; ++i) dp[i] = (seq_t)0;
    for (int x = 0; x < 16; ++x) {
        if (!allowed[x]) continue;
        for (int y = 0; y < 16; ++y) {
            if (!allowed[y]) continue;
            dp[(x << 4) | y] = (seq_t)1;
        }
    }

    /* Iterate for lengths 3..n. */
    for (idx_t len = 3; len <= n; ++len) {
        for (int i = 0; i < 256; ++i) dp_next[i] = (seq_t)0;
        for (int idx = 0; idx < 256; ++idx) {
            seq_t val = dp[idx];
            if (!val) continue;
            int x = idx >> 4;
            int y = idx & 15;
            if (!allowed[x] || !allowed[y]) continue;
            unsigned char cnt = nextcnt[idx];
            for (unsigned char k = 0; k < cnt; ++k) {
                int z = nextz[idx][k];
                dp_next[(y << 4) | z] += val;
            }
        }
        /* swap dp and dp_next */
        for (int i = 0; i < 256; ++i) dp[i] = dp_next[i];
    }

    /* Sum all paths of length n. */
    seq_t ans = (seq_t)0;
    for (int i = 0; i < 256; ++i) ans += dp[i];
    return ans;
}
