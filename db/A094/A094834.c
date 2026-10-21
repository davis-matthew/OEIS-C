/*
 * A094834  Number of (s(0), s(1), ..., s(2n+1)) such that 0 < s(i) < 9 and
 *          |s(i) - s(i-1)| = 1 for i = 1,2,...,2n+1, s(0) = 3, s(2n+1) = 6.
 * https://oeis.org/A094834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 6*a(n-1) - 9*a(n-2) + a(n-3)
 * Initial values: a(1)=1, a(2)=5, a(3)=21
 *
 * This implementation uses memoisation: the values are stored in a static
 * cache array that is expanded on demand.  It works for any seq_t type
 * that supports the arithmetic operators +, -, *, and assignment.
 *
 * The code is valid C11 and C++17.  The cast from void* returned by
 * realloc is explicit to satisfy the C++ compiler.
 */
#include <stdlib.h>
#include <stdint.h>

/* static cache: index 0 is unused, cache[1] holds a(1), etc. */
static seq_t *cache = NULL;                /* array of seq_t */
static idx_t computed_max = 0;             /* largest index already computed */
static size_t cache_size = 0;              /* number of elements allocated */

/* grow the cache to hold indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n < 1) return;                     /* nothing to do */
    /* required array size: indices 0..n, so n+1 elements */
    size_t required = (size_t)n + 1;
    if (cache_size < required) {
        size_t new_size = required > 4 ? required : 4;   /* at least 4 entries */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failure */
        cache      = tmp;
        cache_size = new_size;
    }
}

/* initialise the cache with the first three terms if not already done */
static void init_cache(void)
{
    if (computed_max >= 3) return;          /* already initialised */
    ensure_cache(3);
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)5;
    cache[3] = (seq_t)21;
    computed_max = 3;
}

seq_t a(idx_t n)
{
    if (n < 1) {                            /* sequence undefined for n<1 */
        return (seq_t)0;                    /* arbitrary but defined */
    }

    init_cache();

    if (n <= computed_max)                  /* already computed */
        return cache[n];

    /* extend cache to include n */
    ensure_cache(n);

    /* compute missing values using the recurrence */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t p1 = cache[i - 1];
        seq_t p2 = cache[i - 2];
        seq_t p3 = cache[i - 3];
        /* a(i) = 6*a(i-1) - 9*a(i-2) + a(i-3) */
        seq_t val = p1 * (seq_t)6 - p2 * (seq_t)9 + p3;
        cache[i] = val;
    }

    computed_max = n;
    return cache[n];
}
