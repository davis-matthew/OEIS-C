/*
 * A116920  a(0)=1. a(n) = A020639(a(n-1)) + n.
 * https://oeis.org/A116920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(0) = 1; a(n) = spf(a(n-1)) + n, where spf(m) is the smallest
 *            prime factor of m (intentionally spf(1) = 1)
 * Range:     u8 n <= 96, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required headers */
#include <stdlib.h>
#include <stdint.h>

/* Forward declaration of seq_t and idx_t types,
   which are defined by the build environment. */

/* --------------------------------------------------------------------
 * Smallest prime factor (spf) routine.
 * Works for seq_t values that fit in the integer type.
 * Handles the special case spf(1) = 1.
 * -------------------------------------------------------------------- */
static seq_t spf(seq_t n) {
    /* By convention, spf(1) = 1 */
    if (n <= (seq_t)1) {
        return (seq_t)1;
    }
    /* Even numbers have 2 as the smallest prime factor */
    if ((n & (seq_t)1) == (seq_t)0) {
        return (seq_t)2;
    }

    /* Find the smallest odd divisor of n. */
    for (seq_t i = (seq_t)3; i <= n / i; i += (seq_t)2) {
        if (n % i == (seq_t)0) {
            return i;
        }
    }

    /* n is prime */
    return n;
}

/* --------------------------------------------------------------------
 * Sequence a(n) with memoization.
 * Cache is expanded lazily on first use and whenever a larger
 * index is requested.
 * -------------------------------------------------------------------- */
seq_t a(idx_t n) {
    /* Static cache: array of computed a(n) values. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of valid entries currently stored, always <= cache_cap */
    static idx_t cache_cap = 0;   /* allocated capacity of the cache array */

    /* Allocate initial cache entry for n = 0 if not yet done. */
    if (cache_len == 0) {
        cache_cap = 16; /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) {
            /* In a real build, we might abort or handle this gracefully. */
            exit(EXIT_FAILURE);
        }
        cache[0] = (seq_t)1;  /* a(0) = 1 */
        cache_len = 1;
    }

    /* Expand cache if n exceeds current capacity. */
    while (n >= cache_cap) {
        /* Double the capacity or set to n+1, whichever is larger. */
        idx_t new_cap = cache_cap * 2;
        if (new_cap <= cache_cap) { /* overflow guard */
            new_cap = n + 1;
        }
        if (new_cap < n + 1) {
            new_cap = n + 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev_val = cache[i - 1];
        seq_t f_val = spf(prev_val);
        cache[i] = f_val + (seq_t)i;
    }
    cache_len = n + 1;

    return cache[(size_t)n];
}
