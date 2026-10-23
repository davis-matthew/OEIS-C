/*
 * A196858  Number of n X 3 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 3,4,2,0,1
 *          for x=0,1,2,3,4.
 * https://oeis.org/A196858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of n×3 arrays over {0,1,2,3,4} whose horizontal
 *            and vertical neighbors of each cell x equal 3,4,2,0,1 for
 *            x=0,1,2,3,4. (OEIS A196858)
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 61, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 6‑term linear recurrence:  a(n)=2*a(n-1)-a(n-2)+3*a(n-3)-2*a(n-4)+2*a(n-5)-a(n-6) */

#include <stdint.h>
#include <stdbool.h>

static const unsigned char target_map[5] = {3, 4, 2, 0, 1};

/* ------------------------------------------------------------------ */
/*  Row encoding and transition precomputation                         */
/* ------------------------------------------------------------------ */

static unsigned char row_val[125][3];                /* value of each of the 3 cells   */
static unsigned char trans_next[125][125][125];      /* list of next rows for a transition */
static uint8_t      trans_len[125][125];              /* number of entries in trans_next */

/* precompute all row encodings and transitions */
static void precompute(void)
{
    /* all possible rows (5³) */
    for (int r = 0; r < 125; ++r) {
        int t = r;
        row_val[r][0] = t % 5;  t /= 5;
        row_val[r][1] = t % 5;  t /= 5;
        row_val[r][2] = t % 5;
    }

    /* transition table */
    for (int pp = 0; pp < 125; ++pp) {
        for (int p = 0; p < 125; ++p) {
            uint8_t len = 0;
            for (int nrow = 0; nrow < 125; ++nrow) {
                /* check transition: pp (row‑2), p (row‑1), nrow (current row) */
                bool ok = true;
                const unsigned char *a = row_val[pp];
                const unsigned char *b = row_val[p];
                const unsigned char *c = row_val[nrow];
                for (int j = 0; j < 3 && ok; ++j) {
                    int x  = b[j];
                    int t  = target_map[x];
                    int cnt = 0;
                    if (a[j] == t) ++cnt;
                    if (j > 0 && b[j-1] == t) ++cnt;
                    if (j < 2 && b[j+1] == t) ++cnt;
                    if (c[j] == t) ++cnt;
                    if (cnt != x) ok = false;
                }
                if (ok)
                    trans_next[pp][p][len++] = (unsigned char)nrow;
            }
            trans_len[pp][p] = len;
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Checks for the first and last rows                                 */
/* ------------------------------------------------------------------ */

static inline bool valid_no_up_no_down(int row)
{
    const unsigned char *a = row_val[row];
    for (int j = 0; j < 3; ++j) {
        int x = a[j];
        int t = target_map[x];
        int cnt = 0;
        if (j > 0 && a[j-1] == t) ++cnt;
        if (j < 2 && a[j+1] == t) ++cnt;
        if (cnt != x) return false;
    }
    return true;
}

static inline bool valid_top(int row1, int row2)
{
    const unsigned char *a = row_val[row1];
    const unsigned char *b = row_val[row2];
    for (int j = 0; j < 3; ++j) {
        int x = a[j];
        int t = target_map[x];
        int cnt = 0;
        if (j > 0 && a[j-1] == t) ++cnt;
        if (j < 2 && a[j+1] == t) ++cnt;
        if (b[j] == t) ++cnt;
        if (cnt != x) return false;
    }
    return true;
}

static inline bool valid_bottom(int row_above, int row)
{
    const unsigned char *a = row_val[row_above];
    const unsigned char *b = row_val[row];
    for (int j = 0; j < 3; ++j) {
        int x = b[j];
        int t = target_map[x];
        int cnt = 0;
        if (a[j] == t) ++cnt;
        if (j > 0 && b[j-1] == t) ++cnt;
        if (j < 2 && b[j+1] == t) ++cnt;
        if (cnt != x) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                              */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        precompute();
        initialized = true;
    }

    if (n == 0) return 0;       /* offset is 1, but harmless for n<1 */

    /* n == 1 : only first row, no vertical neighbours */
    if (n == 1) {
        seq_t ans = 0;
        for (int r = 0; r < 125; ++r)
            if (valid_no_up_no_down(r))
                ans += (seq_t)1;
        return ans;
    }

    /* DP tables: dp_prev[p][pp] and dp_cur[p][pp], but we keep
       dp[pp][p] where pp is row‑2, p is row‑1 */
    static seq_t dp[125][125];
    static seq_t dp_next[125][125];

    /* zero dp */
    for (int i = 0; i < 125; ++i)
        for (int j = 0; j < 125; ++j)
            dp[i][j] = 0;

    /* initialization with first two rows */
    for (int r1 = 0; r1 < 125; ++r1)
        for (int r2 = 0; r2 < 125; ++r2)
            if (valid_top(r1, r2))
                dp[r1][r2] = (seq_t)1;

    /* If only two rows are needed, count valid bottoms */
    if (n == 2) {
        seq_t ans = 0;
        for (int pp = 0; pp < 125; ++pp)
            for (int p = 0; p < 125; ++p)
                if (dp[pp][p] && valid_bottom(pp, p))
                    ans += dp[pp][p];
        return ans;
    }

    /* DP propagation for rows 3 .. n */
    for (idx_t row = 3; row <= n; ++row) {
        /* zero dp_next */
        for (int i = 0; i < 125; ++i)
            for (int j = 0; j < 125; ++j)
                dp_next[i][j] = 0;

        for (int pp = 0; pp < 125; ++pp) {
            for (int p = 0; p < 125; ++p) {
                seq_t val = dp[pp][p];
                if (!val) continue;
                uint8_t len = trans_len[pp][p];
                const unsigned char *opts = trans_next[pp][p];
                for (uint8_t k = 0; k < len; ++k) {
                    int nrow = opts[k];
                    dp_next[p][nrow] += val;
                }
            }
        }

        /* copy next to current */
        for (int i = 0; i < 125; ++i)
            for (int j = 0; j < 125; ++j)
                dp[i][j] = dp_next[i][j];
    }

    /* final count: last row is row n, previous row is n-1, before that n-2 */
    seq_t ans = 0;
    for (int pp = 0; pp < 125; ++pp)
        for (int p = 0; p < 125; ++p)
            if (dp[pp][p] && valid_bottom(pp, p))
                ans += dp[pp][p];

    return ans;
}
