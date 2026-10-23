/*
 * A138995  First differences of Frobenius numbers for 4 successive numbers
 *          A138984.
 * https://oeis.org/A138995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-3) - a(n-6); Initial values (offset 1): a(1)=1,
 *            a(2)=1, a(3)=6, a(4)=2, a(5)=2, a(6)=10
 * Range:     u8 n <= 191, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array holding a(1)...a(k) at indices 0..k-1   */
static idx_t cache_alloc = 0;        /* current capacity of cache in terms              */
static idx_t cache_computed = 0;     /* number of terms currently initialized          */

/* Ensure the cache can hold at least n terms (allocate if needed). */
static void ensure_cache_alloc(idx_t n)
{
    if (n <= cache_alloc)
        return;
    seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
    if (!new_cache)
        abort();                      /* out‑of‑memory: terminate */
    cache = new_cache;
    cache_alloc = n;
}

seq_t a(idx_t n)
{
    /* OEIS indexing starts at 1; silence any n < 1 request. */
    if (n == 0)
        return 0;

    /* At least 6 terms are required for the recurrence. */
    idx_t need = n > 6 ? n : 6;
    ensure_cache_alloc(need);

    /* Initialise the first six terms on the first call. */
    if (cache_computed < 6) {
        cache[0] = 1;  /* a(1) */
        cache[1] = 1;  /* a(2) */
        cache[2] = 6;  /* a(3) */
        cache[3] = 2;  /* a(4) */
        cache[4] = 2;  /* a(5) */
        cache[5] = 10; /* a(6) */
        cache_computed = 6;
    }

    /* Return a value that has already been computed. */
    if (n <= cache_computed)
        return cache[n - 1];

    /* Compute new terms up to the requested index using the recurrence. */
    for (idx_t i = cache_computed; i < n; ++i) {
        /* i corresponds to a(i+1); a(i-3) is at index i-3, a(i-6) at index i-6. */
        cache[i] = 2 * cache[i - 3] - cache[i - 6];
    }

    cache_computed = n;
    return cache[n - 1];
}
