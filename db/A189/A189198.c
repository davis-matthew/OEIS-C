/*
 * A189198  Number of 5Xn binary arrays without the pattern 0 0 1 vertically
 *          or antidiagonally.
 * https://oeis.org/A189198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 5×n binary arrays that avoid the vertical *
 *            pattern 0 0 1 and the antidiagonal pattern 0 0 1. The sequence
 *            is * computed by a dynamic‑programming recurrence over the last
 *            two * columns of the array
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

enum { MAX_STATE = 1024, MAX_COL = 32, MAX_TRANS = 32 };

/*  Transition tables – built once. */
static bool trans_done = false;
static int trans_cnt[MAX_STATE];
static int trans[MAX_STATE][MAX_TRANS];

/*  Pre‑computation of vertical‑valid columns and the transition graph. */
static void precompute(void)
{
    static bool vertical_ok[MAX_COL];
    /* determine which single columns are vertical‑valid              */
    for (int c = 0; c < MAX_COL; ++c) {
        bool ok = true;
        for (int r = 0; r < 3 && ok; ++r) {
            int b0 = (c >> r)   & 1;
            int b1 = (c >> (r+1)) & 1;
            int b2 = (c >> (r+2)) & 1;
            if (b0 == 0 && b1 == 0 && b2 == 1) ok = false;
        }
        vertical_ok[c] = ok;
    }

    /* build transition graph for two‑column states                */
    for (int old = 0; old < MAX_STATE; ++old) {
        int p2   = old >> 5;   /* column at distance -2     */
        int p1   = old & 31;   /* column at distance -1     */
        int cnt  = 0;
        for (int curr = 0; curr < MAX_COL; ++curr) {
            if (!vertical_ok[curr]) continue;
            bool bad = false;
            /* check antidiagonal pattern for rows 1‑3                */
            for (int r = 1; r < 4 && !bad; ++r) {
                int bit_p2 = (p2 >> (r-1)) & 1;
                int bit_p1 = (p1 >> r)     & 1;
                int bit_c  = (curr >> (r+1)) & 1;
                if (bit_p2 == 0 && bit_p1 == 0 && bit_c == 1)
                    bad = true;
            }
            if (bad) continue;
            int ns = (p1 << 5) | curr;
            trans[old][cnt++] = ns;
        }
        trans_cnt[old] = cnt;
    }
    trans_done = true;
}

seq_t a(idx_t n)
{
    if (!trans_done) precompute();

    /*  n = 1 : only vertical restrictions apply                */
    if (n == 1) {
        seq_t cnt = 0;
        for (int c = 0; c < MAX_COL; ++c)
            if (trans[0][c] == 0) /* reuse vertical_ok stored in trans[?] */
                cnt += 1;
        /* The above trick is incorrect because trans[0] holds transitions
         * from state 0 (previous two columns 0). Instead we recompute
         * here for clarity.  */
        cnt = 0;
        for (int c = 0; c < MAX_COL; ++c) {
            bool ok = true;
            for (int r = 0; r < 3 && ok; ++r) {
                int b0 = (c >> r)   & 1;
                int b1 = (c >> (r+1)) & 1;
                int b2 = (c >> (r+2)) & 1;
                if (b0 == 0 && b1 == 0 && b2 == 1) ok = false;
            }
            if (ok) cnt += 1;
        }
        return cnt;
    }

    /*  Initialise DP for two columns (n = 2).                     */
    seq_t dp[MAX_STATE];
    for (int i = 0; i < MAX_STATE; ++i) dp[i] = 0;

    for (int c1 = 0; c1 < MAX_COL; ++c1) {
        bool ok1 = true;
        for (int r = 0; r < 3 && ok1; ++r) {
            int b0 = (c1 >> r)   & 1;
            int b1 = (c1 >> (r+1)) & 1;
            int b2 = (c1 >> (r+2)) & 1;
            if (b0 == 0 && b1 == 0 && b2 == 1) ok1 = false;
        }
        if (!ok1) continue;
        for (int c2 = 0; c2 < MAX_COL; ++c2) {
            bool ok2 = true;
            for (int r = 0; r < 3 && ok2; ++r) {
                int b0 = (c2 >> r)   & 1;
                int b1 = (c2 >> (r+1)) & 1;
                int b2 = (c2 >> (r+2)) & 1;
                if (b0 == 0 && b1 == 0 && b2 == 1) ok2 = false;
            }
            if (!ok2) continue;
            int state = (c1 << 5) | c2;
            dp[state] += 1;
        }
    }

    if (n == 2) {
        seq_t total = 0;
        for (int i = 0; i < MAX_STATE; ++i) total += dp[i];
        return total;
    }

    /*  DP for columns 3 … n.                                    */
    seq_t next[MAX_STATE];
    for (idx_t col = 3; col <= n; ++col) {
        for (int i = 0; i < MAX_STATE; ++i) next[i] = 0;
        for (int old = 0; old < MAX_STATE; ++old) {
            seq_t val = dp[old];
            if (val == 0) continue;
            int cnt = trans_cnt[old];
            for (int j = 0; j < cnt; ++j) {
                int ns = trans[old][j];
                next[ns] += val;
            }
        }
        for (int i = 0; i < MAX_STATE; ++i) dp[i] = next[i];
    }

    /*  Sum all end‑states – this is the total number of arrays.     */
    seq_t total = 0;
    for (int i = 0; i < MAX_STATE; ++i) total += dp[i];
    return total;
}
