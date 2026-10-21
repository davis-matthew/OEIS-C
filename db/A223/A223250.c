/*
 * A223250  Two-loop graph coloring a rectangular array: number of n X 3 0..4
 *          arrays where 0..4 label nodes of a graph with edges 0,1 1,2 2,0
 *          0,3 3,4 4,0 and every array movement to a horizontal or vertical
 *          neighbor moves along an edge of this graph.
 * https://oeis.org/A223250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 75*a(n-2) - 126*a(n-3) - 70*a(n-4) + 48*a(n-5)
 *            Initial values: a(1)=32, a(2)=236, a(3)=2172, a(4)=17828,
 *            a(5)=166892 (Valid for n ≥ 1. Behaviour for n < 1 is
 *            unspecified.)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t last_computed = 0;  /* index of highest computed term (or 0 if none) */

/* Allocate or grow cache to accommodate at least 'need' terms */
static void ensure_cache(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap > 0 ? cache_cap : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) using the linear recurrence. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n < 1 */

    /* First call: initialise cache with known initial values */
    if (cache == NULL) {
        ensure_cache(5);
        cache[0] = (seq_t)32;
        cache[1] = (seq_t)236;
        cache[2] = (seq_t)2172;
        cache[3] = (seq_t)17828;
        cache[4] = (seq_t)166892;
        last_computed = 5;
    }

    if (n <= (idx_t)last_computed) {
        return cache[n - 1];
    }

    /* Extend cache to cover up to n */
    ensure_cache((size_t)n);
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i <= 5) {
            /* Already set during initialization; skip */
            continue;
        }
        /* Recurrence indices: cache[i-2] = a(i-1), cache[i-3] = a(i-2), etc. */
        seq_t term = cache[i - 2] * (seq_t)2 + cache[i - 3] * (seq_t)75;
        term = term - cache[i - 4] * (seq_t)126;
        term = term - cache[i - 5] * (seq_t)70;
        term = term + cache[i - 6] * (seq_t)48;
        cache[i - 1] = term;
    }
    last_computed = n;
    return cache[n - 1];
}
