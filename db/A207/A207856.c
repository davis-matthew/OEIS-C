/*
 * A207856  Number of nX6 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=25, a(2)=625; for n>=3 computed by DP over 6‑bit rows
 *            avoiding horizontal 000 and 010, and vertical 3‑row patterns
 *            avoiding 010 and 101. The algorithm stores allowed rows,
 *            precomputes vertical validity, and uses a rolling DP to compute
 *            a(n) for all n
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

/* global state for the DP */
static bool initialized = false;
static unsigned row_count = 0;          /* number of allowed 6‑bit rows (25) */
static uint8_t rows[64];                /* masks of allowed rows */

/* valid_next[p][c][z] == true if rows p, c, z do not contain
   a forbidden vertical 3‑row pattern (010 or 101) */
static bool valid_next[25][25][25];

/* DP tables: dp_prev[p][c] counts entries ending with rows p, c
   after processing the current number of rows.  dp_curr is used
   for the next step. */
static seq_t dp_prev[25][25];
static seq_t dp_curr[25][25];

/* Cache for already computed a(n) values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;        /* number of elements allocated (>= n+1) */
static idx_t cache_max_n = 0;       /* highest n for which cache[n] is valid */

/* Allocate or grow the cache so that indices up to n are valid.
   Newly allocated memory is zero‑initialized. */
static void ensure_cache_size(idx_t n)
{
    if ((size_t)n < cache_cap) return;

    size_t new_cap = n + 1;                     /* need indices 0..n inclusive */
    if (cache == NULL) {
        cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!cache) return;                    /* allocation failed */
        memset(cache, 0, new_cap * sizeof(seq_t));
    } else {
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;                     /* allocation failed */
        /* zero the newly added part */
        memset(tmp + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
        cache = tmp;
    }
    cache_cap = new_cap;
}

/* Build list of allowed rows and the vertical transition table. */
static void init(void)
{
    if (initialized) return;

    /* Generate 25 legal 6‑bit rows (no horizontal 000 or 010). */
    unsigned count = 0;
    for (int i = 0; i < 64; ++i) {
        bool ok = true;
        for (int j = 0; j < 4; ++j) {
            int a = (i >> j) & 1;
            int b = (i >> (j + 1)) & 1;
            int c = (i >> (j + 2)) & 1;
            if ((a == 0 && b == 0 && c == 0) ||
                (a == 0 && b == 1 && c == 0)) {
                ok = false;
                break;
            }
        }
        if (ok) {
            rows[count++] = (uint8_t)i;
        }
    }
    row_count = count;                         /* should be 25 */

    /* Pre‑compute vertical transition table. */
    for (unsigned p = 0; p < row_count; ++p)
        for (unsigned c = 0; c < row_count; ++c)
            for (unsigned z = 0; z < row_count; ++z) {
                bool good = true;
                for (int j = 0; j < 6; ++j) {
                    int xb = (rows[p] >> j) & 1;
                    int yb = (rows[c] >> j) & 1;
                    int zb = (rows[z] >> j) & 1;
                    if ((xb == 0 && yb == 1 && zb == 0) ||
                        (xb == 1 && yb == 0 && zb == 1)) {
                        good = false;
                        break;
                    }
                }
                valid_next[p][c][z] = good;
            }

    initialized = true;
}

/* Main sequence evaluation function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    init();

    ensure_cache_size(n);
    /* if already computed, return it */
    if (cache_max_n >= n) return cache[n];

    /* Compute values up to n, storing in cache. */
    for (idx_t step = cache_max_n + 1; step <= n; ++step) {
        if (step == 1) {            /* a(1) = number of allowed rows */
            cache[1] = (seq_t)row_count;
        } else if (step == 2) {     /* a(2) = row_count^2 */
            for (unsigned p = 0; p < row_count; ++p)
                for (unsigned c = 0; c < row_count; ++c)
                    dp_prev[p][c] = (seq_t)1;
            cache[2] = (seq_t)row_count * row_count;
        } else {                    /* step >= 3 : DP transition */
            /* zero dp_curr */
            for (unsigned i = 0; i < row_count; ++i)
                for (unsigned j = 0; j < row_count; ++j)
                    dp_curr[i][j] = (seq_t)0;

            /* build dp_curr from dp_prev */
            for (unsigned p = 0; p < row_count; ++p)
                for (unsigned c = 0; c < row_count; ++c) {
                    seq_t val = dp_prev[p][c];
                    if (val == (seq_t)0) continue;
                    for (unsigned z = 0; z < row_count; ++z)
                        if (valid_next[p][c][z])
                            dp_curr[c][z] += val;
                }

            /* sum all entries of dp_curr to obtain a(step) */
            seq_t sum = (seq_t)0;
            for (unsigned p = 0; p < row_count; ++p)
                for (unsigned c = 0; c < row_count; ++c)
                    sum += dp_curr[p][c];
            cache[step] = sum;

            /* prepare for next iteration */
            for (unsigned p = 0; p < row_count; ++p)
                for (unsigned c = 0; c < row_count; ++c)
                    dp_prev[p][c] = dp_curr[p][c];
        }
    }
    cache_max_n = n;
    return cache[n];
}
