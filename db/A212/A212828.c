/*
 * A212828  Number of 0..7 arrays of length n with no adjacent pair equal to
 *          its immediately preceding adjacent pair, and new values introduced
 *          in 0..7 order.
 * https://oeis.org/A212828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 0..7 arrays of length n where * 1) values are
 *            introduced in increasing order (the first time * a given value
 *            appears it must be the smallest unused value), * 2) no adjacent
 *            pair equals the adjacent pair two positions earlier. *
 *            Equivalently, for all i ≥ 3, a[i‑2]==a[i] && a[i‑1]=...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 26, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1, not used */
    if (n == 1) return 1;
    if (n == 2) return 2;

    static seq_t dp[8][8][8][8];
    static seq_t next[8][8][8][8];

    /* initialize DP for length 3 sequences */
    memset(dp, 0, sizeof(dp));
    dp[0][0][0][0] = 1;  /* 0,0,0 */
    dp[0][0][1][1] = 1;  /* 0,0,1 */
    dp[0][1][0][1] = 1;  /* 0,1,0 */
    dp[0][1][1][1] = 1;  /* 0,1,1 */
    dp[0][1][2][2] = 1;  /* 0,1,2 */

    /* iterate from length 3 up to n */
    for (idx_t step = 3; step < n; ++step) {
        memset(next, 0, sizeof(next));

        for (int a = 0; a < 8; ++a) {
            for (int b = 0; b < 8; ++b) {
                for (int c = 0; c < 8; ++c) {
                    for (int m = 0; m < 8; ++m) {
                        seq_t val = dp[a][b][c][m];
                        if (!val) continue;

                        /* use existing values 0..m */
                        for (int x = 0; x <= m; ++x) {
                            if (a == c && b == x) continue;   /* forbidden pattern */
                            int na = b, nb = c, nc = x, nm = m;
                            next[na][nb][nc][nm] += val;
                        }

                        /* introduce new value m+1 if possible */
                        if (m < 7) {
                            int x = m + 1;
                            int na = b, nb = c, nc = x, nm = x;
                            /* condition cannot be true because b==x is impossible */
                            next[na][nb][nc][nm] += val;
                        }
                    }
                }
            }
        }
        memcpy(dp, next, sizeof(dp));
    }

    /* sum all states to get the total count */
    seq_t total = 0;
    for (int a = 0; a < 8; ++a)
        for (int b = 0; b < 8; ++b)
            for (int c = 0; c < 8; ++c)
                for (int m = 0; m < 8; ++m)
                    total += dp[a][b][c][m];
    return total;
}
