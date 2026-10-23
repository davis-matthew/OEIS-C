/*
 * A183645  Number of (n+1) X 3 0..4 arrays with every 2 X 2 subblock summing
 *          to 8.
 * https://oeis.org/A183645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*5^n + 24*4^n + 36*3^n + 36*2^n + 24 * Recurrence (valid
 *            for n ≥ 6): * a(n) = 15*a(n-1) - 85*a(n-2) + 225*a(n-3) -
 *            274*a(n-4) + 120*a(n-5) * Initial values: * a(1)=325, a(2)=1001,
 *            a(3)=3445, a(4)=12785, a(5)=50125 * * The implementation caches
 *            computed values so that any call order * take...
 * Range:     u8 none, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

#if defined(__cplusplus)
static seq_t*   cache   = nullptr;
static size_t   cap     = 0;
static idx_t    cache_last = 0;   /* Highest index for which cache[i] is valid */
#else
static seq_t*   cache   = NULL;
static size_t   cap     = 0;
static idx_t    cache_last = 0;   /* Highest index for which cache[i] is valid */
#endif

static void ensure_cache_capacity(idx_t n)
{
    /* Allocate initial cache if needed */
    if (cache == NULL) {
        /* Reserve indices 0..5 (index 0 unused) */
        size_t initial = 6;
#if defined(__cplusplus)
        cache = new seq_t[initial];
#else
        cache = (seq_t*)malloc(initial * sizeof(seq_t));
#endif
        cap = initial;
        /* Set the five known values */
        cache[1] = (seq_t)325;
        cache[2] = (seq_t)1001;
        cache[3] = (seq_t)3445;
        cache[4] = (seq_t)12785;
        cache[5] = (seq_t)50125;
        cache_last = 5;
    }
    /* Grow the array if required */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t* new_cache;
#if defined(__cplusplus)
        new_cache = new seq_t[new_cap];
#else
        new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif
        if (new_cache == NULL) abort();          /* Out of memory          */
        /* Copy old contents */
        for (size_t i = 0; i < cap; ++i) new_cache[i] = cache[i];
        /* Free old storage */
#if defined(__cplusplus)
        delete[] cache;
#else
        free(cache);
#endif
        cache = new_cache;
        cap = new_cap;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* Undefined for non‑positive indices */

    ensure_cache_capacity(n);

    /* Return known value directly if within the first five terms */
    if (n <= 5) return cache[(size_t)n];

    /* Compute missing terms up to n, if any */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        cache[(size_t)i] =
            (seq_t)( (cache[(size_t)(i-1)] * (seq_t)15)
                    - (cache[(size_t)(i-2)] * (seq_t)85)
                    + (cache[(size_t)(i-3)] * (seq_t)225)
                    - (cache[(size_t)(i-4)] * (seq_t)274)
                    + (cache[(size_t)(i-5)] * (seq_t)120) );
    }
    cache_last = n;

    return cache[(size_t)n];
}
