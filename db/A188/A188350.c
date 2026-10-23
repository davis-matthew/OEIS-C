/*
 * A188350  Number of arrangements of n+1 numbers x(i) in -1..1 with the sum
 *          of x(i)*x(i+1) equal to zero.
 * https://oeis.org/A188350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A188350: a(n) = number of arrangements of n+1 numbers x(i)∈{‑1,0,1}
    with Σ_{i=1}^n x(i)·x(i+1)=0.  Computed by dynamic programming over
    the current value and cumulative product sum.  */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* ---------- static state for caching and DP ---------- */
    static seq_t *cache = NULL;          /* cache[k] = a(k) */
    static size_t cache_len = 0;         /* length of cache, index 0 unused */

    static seq_t *dp_prev = NULL;        /* DP after processed edges */
    static seq_t *dp_curr = NULL;        /* scratch DP */
    static size_t dp_size = 0;           /* elements per state (sum range) */
    static int  offset = 0;              /* array index for sum 0 */
    static idx_t processed = 0;          /* number of edges already processed */

    /* ---------- weight table: prev_val * next_val ---------- */
    static const int w[3][3] = {
        {  1,  0, -1 },   /* prev = –1 */
        {  0,  0,  0 },   /* prev =  0 */
        { -1,  0,  1 }    /* prev =  1 */
    };

    /* ---------- extend cache if needed ---------- */
    if (cache_len <= n) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!tmp) return (seq_t)0;   /* allocation failed */
        cache = tmp;
        for (size_t i = cache_len; i <= n; ++i) cache[i] = (seq_t)0;
        cache_len = n + 1;
    }

    /* ---------- expand DP arrays for larger offset ---------- */
    if (offset < (int)n) {
        int new_offset = (int)n;
        size_t new_size = 2 * new_offset + 1;            /* sum indices 0..new_size-1 */

        seq_t *new_prev = (seq_t *)calloc(3 * new_size, sizeof(seq_t));
        seq_t *new_curr = (seq_t *)calloc(3 * new_size, sizeof(seq_t));
        if (!new_prev || !new_curr) return (seq_t)0;

        if (dp_prev) {
            int shift = new_offset - offset;            /* shift existing data */
            for (int st = 0; st < 3; ++st) {
                for (size_t i = 0; i < dp_size; ++i) {
                    new_prev[st * new_size + i + shift] = dp_prev[st * dp_size + i];
                }
            }
        }

        if (!dp_prev) {                                 /* first initialization */
            for (int st = 0; st < 3; ++st)
                new_prev[st * new_size + new_offset] = (seq_t)1;
        }

        free(dp_prev);
        free(dp_curr);
        dp_prev = new_prev;
        dp_curr = new_curr;
        dp_size = new_size;
        offset = new_offset;
    }

    /* ---------- process edges up to n ---------- */
    for (idx_t step = processed + 1; step <= n; ++step) {
        /* clear current DP */
        memset(dp_curr, 0, 3 * dp_size * sizeof(seq_t));

        /* current attainable sum indices range */
        int lower = offset - (int)(step - 1);
        int upper = offset + (int)(step - 1);

        for (int sp = 0; sp < 3; ++sp) {
            seq_t *prev_row = dp_prev + sp * dp_size;
            for (int idx = lower; idx <= upper; ++idx) {
                seq_t val = prev_row[idx];
                if (!val) continue;
                for (int sn = 0; sn < 3; ++sn) {
                    int idx2 = idx + w[sp][sn];
                    dp_curr[sn * dp_size + idx2] += val;
                }
            }
        }

        /* total for sum = 0 (index offset) */
        seq_t total = (seq_t)0;
        for (int sn = 0; sn < 3; ++sn)
            total += dp_curr[sn * dp_size + offset];
        cache[step] = total;

        /* swap DP arrays for next iteration */
        seq_t *tmp = dp_prev;
        dp_prev = dp_curr;
        dp_curr = tmp;
        processed = step;
    }

    return cache[n];
}
