/*
 * A083046  Antidiagonal sums of table A083044.
 * https://oeis.org/A083046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} T(k, n-k) T(k,0) = 3*k + 1 T(k,m+1) = ceil(
 *            (3/2) * T(k,m) ) (for m ≥ 0)
 * Range:     u8 n <= 6, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static state: current maximum n computed, arrays for T(k, n-k) and a-values. */
    static idx_t cur_max   = (idx_t)-1;     /* no values computed yet */
    static seq_t *arr      = NULL;         /* arr[k] = T(k, cur_n - k)  */
    static seq_t *cache    = NULL;         /* cache[i] = a(i)          */
    static size_t cache_cap = 0;           /* capacity of cache array  */
    static size_t arr_cap   = 0;           /* capacity of arr array    */

    /* Helper to grow arrays when needed. */
    if (n >= arr_cap) {
        size_t new_cap = n + 1;
        arr = (seq_t*)realloc(arr, new_cap * sizeof(seq_t));
        if (!arr) abort();  /* graceful exit on failure */
        arr_cap = new_cap;
    }
    if (n >= cache_cap) {
        size_t new_cap = n + 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) abort();
        cache_cap = new_cap;
    }

    /* If we have no data yet, initialize for n = 0. */
    if (cur_max == (idx_t)-1) {
        arr[0]   = 1;                  /* T(0,0) = 1 */
        cache[0] = 1;                  /* a(0) = 1 */
        cur_max  = 0;
    }

    /* Extend computation up to requested n if necessary. */
    while (cur_max < n) {
        idx_t next = cur_max + 1;

        seq_t sum_prev = cache[cur_max];   /* a(next-1)                */
        /* Update all existing rows: T(k, m+1) = ceil(3/2 * T(k,m)). */
        for (idx_t k = 0; k <= cur_max; ++k) {
            seq_t x      = arr[k];                 /* old value   */
            seq_t three_x = 3 * x;                 /* 3*x         */
            seq_t new_x;
            if ((x & 1) != 0)                       /* x odd -> ceil */
                new_x = (three_x + 1) / 2;
            else
                new_x = three_x / 2;
            arr[k] = new_x;
            sum_prev += new_x - x;                  /* delta to sum */
        }

        /* New first entry for the new row: T(next,0) = 3*next + 1. */
        seq_t first = 3 * (seq_t)next + 1;
        arr[next]   = first;
        sum_prev += first;

        cache[next] = sum_prev;                     /* a(next) */
        cur_max    = next;
    }

    return cache[n];
}
