/*
 * A123070  Hofstadter Flip-G-sequence.
 * https://oeis.org/A123070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, a(2)=1, a(3)=2; for n>=4 a(n) = (n+1) - a(
 *            a(n-1) + 1 )
 * Range:     u8 n <= 412, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A123070 (Hofstadter Flip-G-sequence)
   using memoization.  The implementation uses no standard
   library functions beyond malloc/realloc and abort.  The
   recurrence guarantees that all values needed to compute a(n)
   are already computed for indices < n, so an iterative
   fill‑up from the smallest to the requested n is sufficient. */

#include <stdlib.h>

/* Global cache for computed terms */
static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* number of terms currently computed */
static idx_t cache_cap  = 0;   /* allocated capacity (in terms) */

/* Ensure the cache contains the value for n.  If it does not,
   extend the array and compute the missing terms using the
   recurrence. */
static void ensure_cache(idx_t n)
{
    /* Grow the backing array if necessary.  We double the size
       each time to keep amortised linear time. */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 8;
        while (new_cap <= (size_t)n) {
            new_cap <<= 1;
        }
        seq_t *ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!ptr) abort();
        cache = ptr;
        cache_cap = (idx_t)new_cap;
    }

    /* Initialise the known base cases if they have not yet been
       written.  They are required for all subsequent computations. */
    if (cache_size == 0) {
        cache[0] = 0;
        cache_size = 1;
    }
    if (cache_size <= 1 && n >= 1) {
        cache[1] = 1;
        cache_size = 2;
    }
    if (cache_size <= 2 && n >= 2) {
        cache[2] = 1;
        cache_size = 3;
    }
    if (cache_size <= 3 && n >= 3) {
        cache[3] = 2;
        cache_size = 4;
    }

    /* Compute any missing terms, from cache_size up to n inclusive. */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* a(i) = (i+1) - a( a(i-1) + 1 ) */
        /* All intermediate indices are guaranteed to be < i for i>=4. */
        idx_t idx = (idx_t)((seq_t)cache[i-1] + (seq_t)1);
        cache[i] = ((seq_t)(i + 1)) - cache[idx];
    }

    cache_size = n + 1;
}

/* Public function required by the platform. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
