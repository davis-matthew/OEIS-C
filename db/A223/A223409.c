/*
 * A223409  4 X 4 X 4 triangular graph without horizontal edges coloring a
 *          rectangular array: number of n X 2 0..9 arrays where 0..9 label
 *          nodes of a graph with edges 0,1 0,2 1,3 1,4 2,4 2,5 3,6 3,7 4,7
 *          4,8 5,8 5,9 and every array movement to a horizontal or vertical
 *          neighbor moves along an edge of this graph.
 * https://oeis.org/A223409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n‑1) – 17*a(n‑2) – 10*a(n‑3) + 26*a(n‑4) + 10*a(n‑5)
 *            Initial values: a(1)=24, a(2)=132, a(3)=780, a(4)=4664,
 *            a(5)=28060
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t init_vals[5] = {24, 132, 780, 4664, 28060};
static seq_t *cache = NULL;
static size_t cache_len = 0; /* number of terms currently stored (at least 0) */
static size_t cache_cap = 0; /* allocated capacity of cache */

/* Ensure that cache contains terms up to index n (1‑based). */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len) return;            /* already computed */

    size_t needed = (size_t)n;
    if (needed < 5) needed = 5;

    if (needed > cache_cap) {              /* grow cache */
        size_t new_cap = cache_cap ? cache_cap * 2 : 32;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise first five terms if necessary */
    if (cache_len < 5) {
        for (size_t i = cache_len; i < 5 && i < needed; ++i)
            cache[i] = init_vals[i];
        cache_len = 5;
    }

    /* compute remaining terms */
    for (size_t i = cache_len; i < needed; ++i) {
        /* cache indices: a(1) at cache[0] */
        seq_t a1 = cache[i - 1]; /* a(n‑1)   */
        seq_t a2 = cache[i - 2]; /* a(n‑2)   */
        seq_t a3 = cache[i - 3]; /* a(n‑3)   */
        seq_t a4 = cache[i - 4]; /* a(n‑4)   */
        seq_t a5 = cache[i - 5]; /* a(n‑5)   */

        seq_t pos = 9 * a1 + 26 * a4 + 10 * a5;
        seq_t neg = 17 * a2 + 10 * a3;
        cache[i] = pos - neg;
    }
    cache_len = needed;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n<1, return 0 for safety */
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
