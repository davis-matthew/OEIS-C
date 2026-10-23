/*
 * A030067  The "Semi-Fibonacci sequence": a(1) = 1; a(n) = a(n/2) (n even);
 *          a(n) = a(n-1) + a(n-2) (n odd).
 * https://oeis.org/A030067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, look
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 258, u32 n <= 1958, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A030067 – Semi‑Fibonacci sequence
 *
 * Definition:
 *   a(1) = 1
 *   a(n) = a(n/2)        if n is even
 *   a(n) = a(n-1) + a(n-2) if n is odd
 *
 * The implementation uses a small cache that grows lazily.  It works
 * for both C (where seq_t is a builtin unsigned integer type) and
 * C++ (where seq_t can be an arbitrary‑precision big integer class).
 * When compiled as C++ the array is allocated with `new[]`, otherwise
 * with `calloc`.  The cache is zero‑initialised for the C build and
 * value‑initialised for C++.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;          /* cache[0] unused, cache[1] = 1 */
static idx_t   cache_cap   = 0;      /* maximum index that the cache can hold */
static idx_t   computed_cap = 0;      /* largest index that has been filled */

/* Ensure that the cache can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n == 0) return;

    /* No need to grow the cache if it is already large enough. */
    if (cache_cap >= n) return;

    idx_t new_cap = n;
    seq_t *new_cache = NULL;

#ifdef __cplusplus
    /* C++: use new[] (value–initialize the elements). */
    new_cache = new seq_t[new_cap + 1];
    if (cache) {
        /* Copy existing values into the new array. */
        for (idx_t i = 0; i <= cache_cap; ++i) new_cache[i] = cache[i];
    }
#else
    /* C: use calloc which zero‑initialises the memory. */
    new_cache = (seq_t *)calloc((size_t)(new_cap + 1), sizeof(seq_t));
    if (!new_cache) return;  /* allocation failed */
    if (cache) {
        /* Copy existing values.  The remaining new elements are already
         * zeroed by calloc. */
        memcpy(new_cache, cache, (size_t)(cache_cap + 1) * sizeof(seq_t));
    }
#endif

    /* Free the old cache. */
    if (cache) {
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
    }

    cache = new_cache;
    cache_cap = new_cap;

    /* Initialise the first term if it hasn't been set yet. */
    if (computed_cap < 1) {
        cache[1] = (seq_t)1;
        computed_cap = 1;
    }
}

/* Public function: compute the n‑th term of the Semi‑Fibonacci sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined entry, but safe fallback */

    ensure_cache(n);

    /* Return cached value if it has already been computed. */
    if (computed_cap >= n) return cache[n];

    /* Compute all missing entries iteratively. */
    for (idx_t i = computed_cap + 1; i <= n; ++i) {
        if ((i & 1) == 0) {
            /* Even index: a(i) = a(i/2) */
            cache[i] = cache[i >> 1];
        } else {
            /* Odd index: a(i) = a(i-1) + a(i-2) */
            cache[i] = cache[i - 1] + cache[i - 2];
        }
    }
    computed_cap = n;
    return cache[n];
}
