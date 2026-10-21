/*
 * A208038  Number of nX7 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 0 and 1 0 1 vertically.
 * https://oeis.org/A208038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of n×7 0/1 arrays avoiding horizontal 000,010 and
 *            vertical 000,101. Computed by a standard DP on consecutive rows.
 *            a(1) = 40, a(2) = 1600, a(3) = 13230,
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* Maximum possible number of admissible rows (≤40).  */
#define MAX_ROWS   50

static inline bool row_ok(int mask)
{
    for (int i = 0; i <= 4; ++i) {
        int sub = (mask >> i) & 0x7;
        if (sub == 0x0 || sub == 0x2)   /* 000 or 010 forbidden */
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    static bool init = false;
    static int  allowed[MAX_ROWS];
    static int  m;
    static int  nexts[MAX_ROWS][MAX_ROWS][MAX_ROWS];
    static unsigned char next_cnt[MAX_ROWS][MAX_ROWS];

    static seq_t dp_mem[MAX_ROWS][MAX_ROWS];
    static seq_t ndp_mem[MAX_ROWS][MAX_ROWS];
    static seq_t (*dp)[MAX_ROWS] = dp_mem;
    static seq_t (*ndp)[MAX_ROWS] = ndp_mem;

    if (!init) {
        /* Build list of admissible rows */
        m = 0;
        for (int mask = 0; mask < (1 << 7); ++mask) {
            if (row_ok(mask))
                allowed[m++] = mask;
        }

        /* Precompute transitions between consecutive row pairs */
        for (int pi = 0; pi < m; ++pi) {
            int prev = allowed[pi];
            for (int ci = 0; ci < m; ++ci) {
                int curr = allowed[ci];
                unsigned char cnt = 0;
                for (int ni = 0; ni < m; ++ni) {
                    int next = allowed[ni];
                    int all0   = (~prev & ~curr & ~next) & 0x7F;     /* 000 */
                    int one01  = ( prev & (~curr) & next) & 0x7F;    /* 101 */
                    if (all0 == 0 && one01 == 0)
                        nexts[pi][ci][cnt++] = ni;
                }
                next_cnt[pi][ci] = cnt;
            }
        }
        init = true;
    }

    if (n == 1)                  /* a(1) = number of admissible rows */
        return (seq_t)m;

    /* DP for n ≥ 2 : start with all pairs of rows allowed (vertical
       restrictions apply only when a third row is added). */
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < m; ++j)
            dp[i][j] = (seq_t)1;

    /* Dynamic programming over additional rows */
    for (idx_t step = 3; step <= n; ++step) {
        /* Clear target buffer */
        memset(ndp, 0, sizeof(ndp_mem));

        for (int pi = 0; pi < m; ++pi) {
            for (int ci = 0; ci < m; ++ci) {
                seq_t val = dp[pi][ci];
                if (val == (seq_t)0) continue;
                unsigned char cnt = next_cnt[pi][ci];
                for (unsigned char k = 0; k < cnt; ++k) {
                    int ni = nexts[pi][ci][k];
                    ndp[ci][ni] += val;
                }
            }
        }
        /* Advance to next step */
        seq_t (*tmp)[MAX_ROWS] = dp;
        dp = ndp;
        ndp = tmp;
    }

    /* Sum over all ending row pairs */
    seq_t sum = (seq_t)0;
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < m; ++j)
            sum += dp[i][j];

    return sum;
}
