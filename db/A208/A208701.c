/*
 * A208701  Number of 6Xn 0..1 arrays avoiding 0 0 1 and 1 0 0 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A208701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 6×n binary matrices avoiding 001 and 100 *
 *            horizontally and 010 and 101 vertically. * * The algorithm uses
 *            dynamic programming over column pairs. * A column of 6 bits is
 *            *valid* if it contains no vertical * 010 or 101 triple. * A
 *            transition from a pair of columns (prev,curr) to a...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   is_horiz_valid, is_vert_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/is_horiz_valid.h"
#include "../utils/is_vert_valid.h"

enum { COLS   = 64 };                /* number of 6‑bit columns   */
enum { STATES = COLS * COLS };       /* pair of columns          */

static int   VOK[COLS];                     /* vertical‑valid columns   */
static int   trans_cnt[STATES];             /* number of allowed next cols */
static int   trans_to[STATES][COLS];        /* allowed next columns          */

static seq_t cache[100001];     /* cache[1]…cache[max]  (max 100000) */
static idx_t  cache_len = 0;    /* largest index cached      */

static seq_t dp0[STATES];        /* DP arrays for toggling   */
static seq_t dp1[STATES];
static seq_t *dp_cur = dp0;      /* current DP, points to dp0 or dp1 */
static seq_t *dp_next = dp1;     /* next DP                           */

static int init_done = 0;

/* ----------------------------------------------------------------- */

/* ----------------------------------------------------------------- */
/* One‑time precomputation of tables and initial DP values           */
static void precompute(void)
{
    if (init_done) return;
    init_done = 1;

    /* --- vertical validity of columns -------------------------------- */
    for (int col = 0; col < COLS; ++col)
        VOK[col] = is_vert_valid(col);

    /* --- transition tables ------------------------------------------- */
    for (int a = 0; a < COLS; ++a) {
        if (!VOK[a]) continue;
        for (int b = 0; b < COLS; ++b) {
            if (!VOK[b]) continue;
            int idx = a * COLS + b;
            int k   = 0;
            for (int c = 0; c < COLS; ++c) {
                if (!VOK[c]) continue;
                if (is_horiz_valid(a, b, c))
                    trans_to[idx][k++] = c;
            }
            trans_cnt[idx] = k;
        }
    }

    /* --- initial DP for length 2 ------------------------------------ */
    for (int i = 0; i < STATES; ++i)
        dp_cur[i] = 0;
    for (int a = 0; a < COLS; ++a) {
        if (!VOK[a]) continue;
        for (int b = 0; b < COLS; ++b) {
            if (!VOK[b]) continue;
            dp_cur[a * COLS + b] = 1;
        }
    }

    /* --- compute a(1) and a(2) ------------------------------------- */
    seq_t a1 = 0;
    for (int col = 0; col < COLS; ++col)
        if (VOK[col]) a1 += 1;

    seq_t a2 = 0;
    for (int i = 0; i < STATES; ++i)
        a2 += dp_cur[i];

    cache[1] = a1;
    cache[2] = a2;
    cache_len = 2;
}

/* ----------------------------------------------------------------- */
/* Return the n-th term of the sequence (n ≥ 1)                        */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but harmless          */
    precompute();

    if (n <= cache_len)           /* already computed */
        return cache[n];

    /* --- compute terms up to n ------------------------------------- */
    for (idx_t m = cache_len + 1; m <= n; ++m) {
        /* clear dp_next */
        for (int i = 0; i < STATES; ++i)
            dp_next[i] = 0;

        /* transition from dp_cur (length m-1) to dp_next (length m) */
        for (int a = 0; a < COLS; ++a) {
            if (!VOK[a]) continue;
            for (int b = 0; b < COLS; ++b) {
                if (!VOK[b]) continue;
                seq_t cnt = dp_cur[a * COLS + b];
                if (!cnt) continue;
                int idx = a * COLS + b;
                int k   = trans_cnt[idx];
                for (int t = 0; t < k; ++t) {
                    int c = trans_to[idx][t];
                    dp_next[b * COLS + c] += cnt;
                }
            }
        }

        /* sum of dp_next entries is a(m) */
        seq_t sum = 0;
        for (int i = 0; i < STATES; ++i)
            sum += dp_next[i];

        cache[m] = sum;

        /* swap dp_cur and dp_next for next iteration */
        seq_t *tmp   = dp_cur;
        dp_cur       = dp_next;
        dp_next      = tmp;
    }

    cache_len = n;
    return cache[n];
}
