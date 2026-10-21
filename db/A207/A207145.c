/*
 * A207145  Number of (n+1) X 5 0..2 arrays with every 2 X 2 subblock having
 *          nonzero determinant and commuting with every horizontal or
 *          vertical neighbor.
 * https://oeis.org/A207145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - 2*a(n-3) for n > 12 with initial
 *            values a(1)=62, a(2)=95, a(3)=110, a(4)=190, a(5)=218, a(6)=374,
 *            a(7)=434, a(8)=742, a(9)=866, a(10)=1478, a(11)=1730, a(12)=2954
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 53, u64 n <= 117, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Two helper macros for the indices */
#define MAX_INIT_IDX   12

/* Hard‑coded initial values – we rely on seq_t being large enough
   for the supplied numbers.  The build is expected to provide a
   seq_t capable of holding all terms that are guaranteed to fit. */
static const seq_t init_terms[MAX_INIT_IDX + 1] = {
    0,    /* unused, 0‑based placeholder */
    62, 95, 110, 190, 218, 374, 434, 742, 866, 1478, 1730, 2954
};

static seq_t *cache = NULL;   /* 1‑based cache of computed terms */
static size_t cap = 0;        /* current allocated size (0 = unallocated) */
static idx_t max_comp = 0;    /* highest n that has been filled */

/* Ensure the cache has at least (n+1) elements (index 0 unused). */
static void ensure_cache_cap(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cap)
        return;

    /* Grow the cache.  Use realloc; if it fails, we abort – the
       sequence generation must not silently return wrong values. */
    void *tmp = realloc(cache, needed * sizeof(seq_t));
    if (!tmp) {
        /* In production code one would handle the error; here we
           simply let the program terminate. */
        abort();
    }
    cache = (seq_t *)tmp;
    /* Zero out the newly allocated space to keep uninitialised
       data out of the cache – not strictly necessary but clean. */
    for (size_t i = cap; i < needed; ++i)
        cache[i] = 0;
    cap = needed;
}

/* Compute all terms up to n inclusive, storing them in cache. */
static void compute_up_to(idx_t n)
{
    if (n <= max_comp)
        return;

    ensure_cache_cap(n);
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        if (i <= MAX_INIT_IDX)
            cache[i] = init_terms[i];
        else {
            /* a(i) = a(i-1) + 2*a(i-2) – 2*a(i-3) */
            seq_t v = cache[i - 1];
            v += cache[i - 2] << 1;
            v -= cache[i - 3] << 1;
            cache[i] = v;
        }
    }
    max_comp = n;
}

/* Public entry point for the sequence.  The function is safe for
   arbitrary ordering of successive calls. */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; undefined otherwise. */
    if (n == 0)
        return 0;

    compute_up_to(n);
    return cache[n];
}
