/*
 * A038990  Expansion of (1-x-x^2+2*x^3) / ((1-x)*(1+x)*(1-3*x+x^2)).
 * https://oeis.org/A038990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-3) + a(n-4), with a(0)=1, a(1)=2,
 *            a(2)=5, a(3)=14. The sequence grows monotonically; values fit in
 *            seq_t as long as the built‑in integer types allow it
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoisation cache */
static seq_t *cache = NULL;
static idx_t cache_size = 0;  /* number of terms stored (first cache_size indices are valid) */
static idx_t cache_cap  = 0;  /* current allocation size */

/* Ensure the cache can hold at least (needed+1) terms. */
static void ensure_cache(idx_t needed)
{
    if (needed < cache_cap)
        return;

    size_t new_cap = cache_cap ? (size_t)cache_cap << 1 : 8;
    while (new_cap <= (size_t)needed)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();                     /* unrecoverable memory allocation failure */

    cache   = new_cache;
    cache_cap = (idx_t)new_cap;
}

/* Compute all terms up to and including index n, storing them in the cache. */
static void compute_up_to(idx_t n)
{
    if (n < cache_size)               /* already computed */
        return;

    /* Initialise the first four terms on the first call. */
    if (cache_size == 0) {
        ensure_cache(3);
        cache[0] = 1;
        cache[1] = 2;
        cache[2] = 5;
        cache[3] = 14;
        cache_size = 4;
    }

    /* Make sure the array is large enough. */
    ensure_cache(n);

    /* Compute missing terms using the linear recurrence. */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t tmp = cache[i - 1];
        tmp = tmp * 3;                       /* 3*a(i-1) */
        tmp -= cache[i - 3] * 3;              /* -3*a(i-3) */
        tmp += cache[i - 4];                  /* +a(i-4)  */
        cache[i] = tmp;
    }

    if (n + 1 > cache_size)
        cache_size = n + 1;
}

/* Public function supplied to the build system. */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[n];
}
