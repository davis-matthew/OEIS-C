/*
 * A175934  Number of lattice paths from (0,0) to (n,n) using steps
 *          S={(1,0),(0,1),(r,r)|0<r<=2} that never go above the line y=x.
 * https://oeis.org/A175934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 27, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A175934:
 * a(0) = 1, a(1) = 2
 * a(n) = a(n-1) + a(n-2) + c[n-1]  for n >= 2
 * where c[m] = Σ_{k=0..m} a(k) * a(m-k)  (self‑convolution of the sequence)
 * This recurrence follows from the definition of the lattice paths.
 */

#include <stddef.h>
#include <stdlib.h>

/* Static cache for the sequence (a) and its convolution (c). */
static seq_t *a_cache = NULL;   /* a[n] values            */
static seq_t *c_cache = NULL;   /* c[n] = Σ a[k]a[n-k]    */
static size_t cache_size = 0;   /* number of a terms in cache */
static size_t cache_cap   = 0;   /* allocated capacity         */

/* Ensure the cache has space for at least 'need' elements. */
static void ensure_capacity(size_t need) {
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? (cache_cap << 1) : 2;
        if (new_cap < need) new_cap = need;
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        c_cache = (seq_t *)realloc(c_cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* Initialize the cache with the first two terms. */
static void init_cache(void) {
    if (cache_size == 0) {
        ensure_capacity(2);
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)2;
        /* convolution for n=0 and n=1 */
        c_cache[0] = a_cache[0] * a_cache[0];          /* 1 */
        c_cache[1] = a_cache[0] * a_cache[1] +
                      a_cache[1] * a_cache[0];          /* 4 */
        cache_size = 2;
    }
}

/* Compute all terms up to and including index n. */
static void compute_up_to(idx_t n) {
    size_t target = (size_t)n + 1;   /* number of terms needed   */
    ensure_capacity(target);
    init_cache();

    /* Compute terms incrementally. */
    for (size_t m = cache_size; m < target; ++m) {
        /* a[m] = a[m-1] + a[m-2] + c[m-1]  (turns into a loop O(n^2) overall) */
        a_cache[m] = a_cache[m-1] + a_cache[m-2] + c_cache[m-1];

        /* compute convolution c[m] = Σ_{k=0..m} a[k]*a[m-k] */
        seq_t sum = (seq_t)0;
        for (size_t k = 0; k <= m; ++k) {
            sum += a_cache[k] * a_cache[m - k];
        }
        c_cache[m] = sum;
    }
    cache_size = target;
}

/* Public entry point */
seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)2;
    compute_up_to(n);
    return a_cache[(size_t)n];
}
