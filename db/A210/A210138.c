/*
 * A210138  Number of (n+1)X4 0..3 arrays containing all values 0..3 with
 *          every 2X2 subblock having one, two or four distinct values, and
 *          new values 0..3 introduced in row major order.
 * https://oeis.org/A210138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computation of OEIS sequence A210138
 *  Number of (n+1)×4 0..3 arrays containing all values 0..3
 *  with every 2×2 subblock having one, two or four distinct values,
 *  and new values 0..3 introduced in row‑major order.
 *
 *  This implementation uses dynamic programming over rows.
 *  States encode the current row pattern (4 cells) and the
 *  current maximum value seen in the array.
 *  Pre‑computed compatibility tables allow transitions between
 *  consecutive rows in O(1) per state.
 *  DP is memoised so that any call a(n) runs in O(n) time.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define STATE_COUNT 1024           /* 256 patterns × 4 possible max values */

/* ------------------------------------------------------------------ */
/* Pre‑computed tables                                                  */
static unsigned char pattern_first[256];
static signed char   next_m[256][4];          /* -1 if impossible */
static signed char   next_count[STATE_COUNT];
static unsigned short next_ids[STATE_COUNT][256];

/* ------------------------------------------------------------------ */
/* Helper to initialise tables once                                    */
static bool tables_ready = false;

static void init_tables(void)
{
    /* compute data for each pattern */
    for (int p = 0; p < 256; ++p) {
        int v0 = (p >> 0) & 3;
        int v1 = (p >> 2) & 3;
        int v2 = (p >> 4) & 3;
        int v3 = (p >> 6) & 3;
        pattern_first[p] = (unsigned char)v0;

        for (int m = 0; m < 4; ++m) {          /* current maximum value */
            int cur = m;
            bool ok = true;
            for (int idx = 0; idx < 4; ++idx) {
                int val = (p >> (idx * 2)) & 3;
                if (val > cur + 1) { ok = false; break; }
                if (val == cur + 1) cur = val;
            }
            next_m[p][m] = ok ? (signed char)cur : (signed char)-1;
        }
    }

    /* build transition graph */
    for (int p = 0; p < 256; ++p) {
        for (int m = 0; m < 4; ++m) {
            int id = (p << 2) | m;
            int cnt = 0;
            for (int q = 0; q < 256; ++q) {
                /* check every 2×2 block allowed values */
                bool ok = true;
                for (int col = 0; col < 3 && ok; ++col) {
                    int a0 = (p >> (col * 2)) & 3;
                    int a1 = (p >> ((col + 1) * 2)) & 3;
                    int b0 = (q >> (col * 2)) & 3;
                    int b1 = (q >> ((col + 1) * 2)) & 3;
                    unsigned mask = (1u << a0) | (1u << a1) |
                                    (1u << b0) | (1u << b1);
                    if (__builtin_popcount(mask) == 3) ok = false;
                }
                if (!ok) continue;

                signed char m2 = next_m[q][m];
                if (m2 < 0) continue;          /* violates row‑major rule */
                next_ids[id][cnt++] = (unsigned short)((q << 2) | m2);
            }
            next_count[id] = (signed char)cnt;
        }
    }
}

/* ------------------------------------------------------------------ */
/* DP storage and memoised values                                     */
static seq_t dp[STATE_COUNT];
static seq_t dp_next[STATE_COUNT];

static seq_t *cache_vals = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t rows_done = 0;          /* number of rows processed */

/* ensure cache can hold values up to n */
static void ensure_cache(idx_t n)
{
    if ((size_t)(n + 1) <= cache_cap) return;
    size_t newcap = (size_t)(n + 1);
    seq_t *newbuf = (seq_t*)calloc(newcap, sizeof(seq_t));
    if (cache_vals && cache_len) memcpy(newbuf, cache_vals,
                                        cache_len * sizeof(seq_t));
    free(cache_vals);
    cache_vals = newbuf;
    cache_cap = newcap;
    cache_len = cache_len > newcap ? newcap : cache_len;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                              */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* a(0) is outside the OEIS offset */

    if (!tables_ready) { init_tables(); tables_ready = true;}

    /* initialise DP for the first row if not already done */
    if (rows_done == 0) {
        for (int i = 0; i < STATE_COUNT; ++i) dp[i] = (seq_t)0;
        for (int p = 0; p < 256; ++p) {
            if (pattern_first[p] != 0) continue;
            signed char m2 = next_m[p][0];
            if (m2 < 0) continue;
            unsigned id = (unsigned)(p << 2) | (unsigned)m2;
            dp[id] += (seq_t)1;
        }
        rows_done = 1;
        ensure_cache(0);
        cache_vals[0] = (seq_t)0;   /* no complete array after 1 row */
        cache_len = 1;
    }

    /* extend DP until we reach processed rows == n+1 */
    while (rows_done <= n) {
        for (int i = 0; i < STATE_COUNT; ++i) dp_next[i] = (seq_t)0;

        for (int id = 0; id < STATE_COUNT; ++id) {
            seq_t val = dp[id];
            if (val == (seq_t)0) continue;
            int cnt = next_count[id];
            unsigned short *lst = next_ids[id];
            for (int k = 0; k < cnt; ++k) {
                dp_next[lst[k]] += val;
            }
        }

        /* swap */
        for (int i = 0; i < STATE_COUNT; ++i) dp[i] = dp_next[i];
        ++rows_done;

        /* compute a(rows_done-1) – number of arrays that now contain all 0..3 */
        seq_t sum = 0;
        for (int id = 0; id < STATE_COUNT; ++id)
            if ((id & 3) == 3) sum += dp[id];

        ensure_cache(rows_done - 1);
        cache_vals[rows_done - 1] = sum;
        if (rows_done > cache_len) cache_len = rows_done;
    }

    return cache_vals[n];
}
