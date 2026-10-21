/*
 * A234118  Number of (n+1)X(4+1) 0..2 arrays with every 2X2 subblock having
 *          the absolute values of all six edge and diagonal differences no
 *          larger than 1.
 * https://oeis.org/A234118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A234118 : number of (n+1)×5 0..2 arrays with all 2×2 subblocks having
   absolute differences of all six edge and diagonal pairs ≤ 1.
   Computed by dynamic programming over admissible row patterns.
*/
#include <stdlib.h>
#include <stdbool.h>
#ifdef __cplusplus
# include <vector>
#endif

/* Maximum number of possible 5‑cell patterns with values 0..2 that satisfy
   the horizontal difference constraint. */
#define MAX_PATTERNS 243

static bool initialized = false;
static int pattern_count = 0;
static uint8_t patterns[MAX_PATTERNS][5];
static uint8_t trans[MAX_PATTERNS][MAX_PATTERNS];

#if defined(__cplusplus)
static std::vector<seq_t> v_cur;
static std::vector<seq_t> v_next;
static std::vector<seq_t> cache;
#else
static seq_t *v_cur = NULL;
static seq_t *v_next = NULL;
static seq_t *cache = NULL;
static size_t cache_cap = 0;
#endif

static size_t rows_done = 1;   /* how many rows (height) have already been
                                 processed; 1 after initialization */

/*------------------------------------------------------------------*/
/* Ensure that the cache is large enough to hold values up to n.    */
#if defined(__cplusplus)
/* C++ version: use std::vector */
static void ensure_cache(size_t n)
{
    if (cache.size() <= n) cache.resize(n + 1, (seq_t)0);
}
#else
/* C version: use realloc on a raw array */
static void ensure_cache(size_t n)
{
    if (cache_cap <= n) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }
}
#endif
/*------------------------------------------------------------------*/
/* Build all admissible row patterns and the transition matrix.    */
static void init(void)
{
    /* Generate row patterns that satisfy the horizontal adjacency rule. */
    int idx = 0;
    for (int a0 = 0; a0 <= 2; ++a0)
    for (int a1 = 0; a1 <= 2; ++a1) if (abs(a0 - a1) <= 1)
    for (int a2 = 0; a2 <= 2; ++a2) if (abs(a1 - a2) <= 1)
    for (int a3 = 0; a3 <= 2; ++a3) if (abs(a2 - a3) <= 1)
    for (int a4 = 0; a4 <= 2; ++a4) if (abs(a3 - a4) <= 1)
    {
        patterns[idx][0] = (uint8_t)a0;
        patterns[idx][1] = (uint8_t)a1;
        patterns[idx][2] = (uint8_t)a2;
        patterns[idx][3] = (uint8_t)a3;
        patterns[idx][4] = (uint8_t)a4;
        ++idx;
    }
    pattern_count = idx;

    /* Transition matrix: trans[i][j] = 1 if rows i and j can follow each other
       (vertical and diagonal constraints). */
    for (int i = 0; i < pattern_count; ++i)
    for (int j = 0; j < pattern_count; ++j) {
        bool ok = true;
        /* vertical differences */
        for (int c = 0; c < 5 && ok; ++c)
            if (abs((int)patterns[i][c] - (int)patterns[j][c]) > 1)
                ok = false;
        /* two diagonal differences per column pair */
        for (int c = 0; c < 4 && ok; ++c)
        {
            if (abs((int)patterns[i][c] - (int)patterns[j][c+1]) > 1) ok = false;
            if (abs((int)patterns[i][c+1] - (int)patterns[j][c]) > 1) ok = false;
        }
        trans[i][j] = ok ? 1 : 0;
    }

    /* Allocate DP vectors. */
#if defined(__cplusplus)
    v_cur.assign(pattern_count, (seq_t)1);
    v_next.assign(pattern_count, (seq_t)0);
    cache.resize(2, (seq_t)0);
    cache[1] = (seq_t)pattern_count;   /* value overwritten in first call */
    rows_done = 1;
#else
    v_cur = (seq_t*)malloc(pattern_count * sizeof(seq_t));
    v_next = (seq_t*)malloc(pattern_count * sizeof(seq_t));
    if (!v_cur || !v_next) abort();
    for (int i = 0; i < pattern_count; ++i) v_cur[i] = (seq_t)1;
    for (int i = 0; i < pattern_count; ++i) v_next[i] = (seq_t)0;
    ensure_cache(1);
    cache[1] = (seq_t)pattern_count;   /* overwritten later */
#endif

    initialized = true;
}
/*------------------------------------------------------------------*/
/* External linkage function required by the build.               */
seq_t a(idx_t n)
{
    if (!initialized) init();
    size_t idx = (size_t)n;   /* make an ordinary size_t for array indexing */

    ensure_cache(idx);

    while (rows_done <= idx)
    {
        /* v_next = v_cur * trans  (simple DP, all values are non‑negative) */
#if defined(__cplusplus)
        std::fill(v_next.begin(), v_next.end(), (seq_t)0);
#else
        for (int j = 0; j < pattern_count; ++j) v_next[j] = (seq_t)0;
#endif
        for (int i = 0; i < pattern_count; ++i)
            if (v_cur[i]) {
                for (int j = 0; j < pattern_count; ++j)
                    if (trans[i][j]) v_next[j] += v_cur[i];
            }

        /* Sum all ways for this height. */
        seq_t total = (seq_t)0;
        for (int j = 0; j < pattern_count; ++j) total += v_next[j];
        cache[rows_done] = total;   /* store the a(rows_done) value */

        /* Prepare for next iteration: v_cur = v_next. */
#if defined(__cplusplus)
        v_cur.swap(v_next);
#else
        for (int j = 0; j < pattern_count; ++j) v_cur[j] = v_next[j];
#endif
        ++rows_done;
    }
    return cache[idx];
}
