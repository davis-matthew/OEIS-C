/*
 * A204752  Number of (n+2) X 8 0..1 arrays with every 3 X 3 subblock having
 *          three equal elements in a row horizontally, vertically, diagonally
 *          or antidiagonally exactly three ways, and new values 0..1
 *          introduced in row major order.
 * https://oeis.org/A204752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) – a(n-3) for n>5; Initial values: a(1)=590,
 *            a(2)=49, a(3)=48, a(4)=53, a(5)=61
 * Range:     u8 none, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static storage for memoised terms; index 1..count are valid */
static seq_t *a_cache = NULL;
static size_t a_cache_cap = 0;      /* allocated size of a_cache */
static idx_t  a_cache_count = 0;     /* highest index that has been calculated */

/* enlarge the cache so that it can hold index 'need' (1‑based) */
static void a_ensure_size(idx_t need)
{
    if ((size_t)need + 1 <= a_cache_cap) return;

    size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 8;
    while ((size_t)need + 1 > new_cap) new_cap *= 2;

    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    a_cache_cap = new_cap;
}

/* fill the cache up to index 'n' (inclusive) */
static void a_compute_to(idx_t n)
{
    if (n <= a_cache_count) return;

    /* ensure storage */
    a_ensure_size(n);

    /* initialise first five values once */
    if (a_cache_count < 1) a_cache[1] = (seq_t)590;
    if (a_cache_count < 2) a_cache[2] = (seq_t)49;
    if (a_cache_count < 3) a_cache[3] = (seq_t)48;
    if (a_cache_count < 4) a_cache[4] = (seq_t)53;
    if (a_cache_count < 5) a_cache[5] = (seq_t)61;

    /* compute missing terms using the recurrence */
    for (idx_t i = a_cache_count + 1; i <= n; ++i) {
        if (i <= 5) continue;          /* already set */
        seq_t prev  = a_cache[i-1];    /* a(i-1)   */
        seq_t prev3 = a_cache[i-3];    /* a(i-3)   */
        a_cache[i] = (seq_t)((seq_t)2 * prev - prev3);
    }

    a_cache_count = n;
}

/* public function returning the n‑th term (n≥1) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* out of bounds: defined as 0 */
    a_compute_to(n);
    return a_cache[n];
}
