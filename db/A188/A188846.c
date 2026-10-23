/*
 * A188846  Number of nX4 binary arrays without the pattern 0 0 1 vertically,
 *          antidiagonally or horizontally.
 * https://oeis.org/A188846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A188846: Number of n×4 binary arrays without the pattern
 *  0 0 1 vertically, antidiagonally or horizontally.
 *  Computed by a 256‑state transfer‑matrix dynamic program.
 */
#include <stdlib.h>
#include <string.h>

/*------------------------------------   helper data   ------------------------------------*/
static int valid_rows[16];
static int valid_cnt = 0;
static unsigned char trans_next[256][16];
static unsigned char trans_count[256];

/*------------------------------------   helpers   ------------------------------------*/
static int row_valid(int r)
{
    int j;
    for (j = 0; j <= 1; ++j) {
        int b0 = (r >> j) & 1;
        int b1 = (r >> (j + 1)) & 1;
        int b2 = (r >> (j + 2)) & 1;
        if (b0 == 0 && b1 == 0 && b2 == 1)
            return 0;
    }
    return 1;
}

static int transition_ok(int r1, int r2, int r3)
{
    int j;
    /* vertical 0 0 1 */
    for (j = 0; j < 4; ++j) {
        int b1 = (r1 >> j) & 1;
        int b2 = (r2 >> j) & 1;
        int b3 = (r3 >> j) & 1;
        if (b1 == 0 && b2 == 0 && b3 == 1)
            return 0;
    }
    /* antidiagonal 0 0 1 */
    for (j = 0; j < 2; ++j) {
        int b1 = (r1 >> (j + 2)) & 1;
        int b2 = (r2 >> (j + 1)) & 1;
        int b3 = (r3 >> j) & 1;
        if (b1 == 0 && b2 == 0 && b3 == 1)
            return 0;
    }
    return 1;
}

/*------------------------------------   initialization   ------------------------------------*/
static void init_matrix(void)
{
    int r;
    /* collect valid rows */
    for (r = 0; r < 16; ++r)
        if (row_valid(r))
            valid_rows[valid_cnt++] = r;

    /* build transition tables */
    int r1, r2, r3;
    for (r1 = 0; r1 < 16; ++r1)
        for (r2 = 0; r2 < 16; ++r2)
            for (r3 = 0; r3 < 16; ++r3)
                if (row_valid(r3) && transition_ok(r1, r2, r3)) {
                    int state = (r1 << 4) | r2;
                    int idx = trans_count[state]++;
                    trans_next[state][idx] = (unsigned char)r3;
                }
}

/*------------------------------------   global state   ------------------------------------*/
static seq_t *cache_seq = NULL;       /* cache[n] = a(n)  (cache_seq[0] unused)      */
static idx_t cache_cap = 0;            /* allocated size of cache_seq                 */
static idx_t max_computed = 0;         /* highest n for which cache_seq[n] is valid   */

static seq_t dp[256];
static seq_t next_dp[256];
static int initialized = 0;

/* ensure cache has space up to n */
static void ensure_cache(idx_t n)
{
    if (n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache_seq, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache_seq = tmp;
        cache_cap = new_cap;
    }
}

/* compute all values up to n */
static void compute_to(idx_t n)
{
    int i, s, k, r1, r2, c;
    while (max_computed < n) {
        memset(next_dp, 0, sizeof(next_dp));
        for (s = 0; s < 256; ++s) {
            if (dp[s] == 0) continue;
            r1 = s >> 4;
            r2 = s & 15;
            for (k = 0; k < trans_count[s]; ++k) {
                c = trans_next[s][k];
                int ns = (r2 << 4) | c;
                next_dp[ns] += dp[s];
            }
        }
        /* sum all states to get a(max_computed+1) */
        seq_t sum = 0;
        for (i = 0; i < 256; ++i) sum += next_dp[i];
        ensure_cache(max_computed + 1);
        cache_seq[max_computed + 1] = sum;

        /* prepare for next iteration */
        memcpy(dp, next_dp, sizeof(dp));
        ++max_computed;
    }
}

/*------------------------------------   public function   ------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined but harmless */

    if (!initialized) {
        init_matrix();
        ensure_cache(2);
        cache_seq[1] = (seq_t)valid_cnt;
        cache_seq[2] = (seq_t)valid_cnt * (seq_t)valid_cnt;
        memset(dp, 0, sizeof(dp));
        int i, j;
        for (i = 0; i < valid_cnt; ++i)
            for (j = 0; j < valid_cnt; ++j) {
                int idx = (valid_rows[i] << 4) | valid_rows[j];
                dp[idx] += 1;
            }
        max_computed = 2;
        initialized = 1;
    }

    if (n <= max_computed) return cache_seq[n];
    compute_to(n);
    return cache_seq[n];
}
