/*
 * A188750  Number of 5Xn binary arrays without the pattern 1 1 1 diagonally,
 *          vertically or antidiagonally.
 * https://oeis.org/A188750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A188750
    Number of 5×n binary arrays without the pattern 1 1 1 vertically, diagonally or anti‑diagonally.
    Computation is performed by dynamic programming over the 5‑bit column states.   */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /*  Pre‑compute allowed column patterns and transitions once.  */
    static bool init_done = false;
    static unsigned char allowed[32];               /* allowed column masks */
    static unsigned char succ_cnt[32][32];          /* number of successors for pair (s1,s2) */
    static unsigned char succ[32][32][32];          /* list of successors for pair (s1,s2) */

    if (!init_done) {
        /*  Column pattern must not contain three consecutive 1’s vertically.  */
        for (int s = 0; s < 32; ++s) {
            unsigned char v = !(
                ((s & 0b111) == 0b111) ||
                (((s >> 1) & 0b111) == 0b111) ||
                (((s >> 2) & 0b111) == 0b111));
            allowed[s] = v ? 1 : 0;
        }

        /*  Pre‑compute transitions.  */
        for (int s1 = 0; s1 < 32; ++s1) if (allowed[s1]) {
            for (int s2 = 0; s2 < 32; ++s2) if (allowed[s2]) {
                int c = 0;
                for (int s3 = 0; s3 < 32; ++s3) if (allowed[s3]) {
                    bool bad = false;
                    /* main diagonal */
                    for (int r = 0; r < 3 && !bad; ++r)
                        if (((s1 >> r) & 1) &&
                            ((s2 >> (r + 1)) & 1) &&
                            ((s3 >> (r + 2)) & 1))
                            bad = true;
                    /* anti‑diagonal */
                    for (int r = 2; r < 5 && !bad; ++r)
                        if (((s1 >> r) & 1) &&
                            ((s2 >> (r - 1)) & 1) &&
                            ((s3 >> (r - 2)) & 1))
                            bad = true;
                    if (!bad) {
                        succ[s1][s2][c++] = (unsigned char)s3;
                    }
                }
                succ_cnt[s1][s2] = c;
            }
        }
        init_done = true;
    }

    if (n == 0) return 0;   /* undefined, return 0 for safety */

    /*  n = 1  : count of allowed columns.  */
    if (n == 1) {
        seq_t total = 0;
        for (int s = 0; s < 32; ++s) if (allowed[s]) total += (seq_t)1;
        return total;
    }

    /*  DP over the last two columns.  */
    static seq_t dp[2][32][32];   /* two layers that are swapped */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j) {
            dp[0][i][j] = 0;
            dp[1][i][j] = 0;
        }

    /*  Initialise for length 2 (no diagonal constraints apply yet).  */
    for (int s1 = 0; s1 < 32; ++s1) if (allowed[s1])
        for (int s2 = 0; s2 < 32; ++s2) if (allowed[s2])
            dp[0][s1][s2] = 1;

    if (n == 2) {
        seq_t total = 0;
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j) total += dp[0][i][j];
        return total;
    }

    int cur = 0, nxt = 1;
    for (idx_t step = 3; step <= n; ++step) {
        /*  zero next layer  */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j) dp[nxt][i][j] = 0;

        for (int s1 = 0; s1 < 32; ++s1) if (allowed[s1])
            for (int s2 = 0; s2 < 32; ++s2) if (allowed[s2]) {
                seq_t base = dp[cur][s1][s2];
                if (base == 0) continue;
                int c = succ_cnt[s1][s2];
                for (int t = 0; t < c; ++t) {
                    int s3 = succ[s1][s2][t];
                    dp[nxt][s2][s3] += base;
                }
            }

        /*  swap layers  */
        int tmp = cur; cur = nxt; nxt = tmp;
    }

    /*  sum all end states  */
    seq_t total = 0;
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j) total += dp[cur][i][j];
    return total;
}
