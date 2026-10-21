/*
 * A083238  First order recursion: a(0)=1; a(n) = sigma(1,n) - a(n-1).
 * https://oeis.org/A083238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; a(n) = σ(n) – a(n–1) (for n ≥ 1), where σ(n) = sum of
 *            positive divisors of n
 * Range:     i8 n <= 23, i16 n <= 397, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#if !defined(__cplusplus)
#include <string.h>   /* memcpy */
#endif

/* Static cache of already computed a(n) values.  */
static seq_t *a_cache = NULL;      /* array of a values */
static size_t cache_size = 0;      /* number of elements stored (>= 1 after first call) */

/* Extend the cache so that index n is available.  */
static void extend_to(idx_t n)
{
    /* Nothing to do if we already have the value. */
    if ((size_t)n < cache_size)
        return;

    size_t new_size = (size_t)n + 1;          /* need indices 0 … n inclusive */
    seq_t *new_cache = NULL;

#if !defined(__cplusplus)
    /* C implementation – the type seq_t is a plain integer type.  */
    new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (new_cache == NULL)
        abort();
    if (a_cache != NULL) {
        memcpy(new_cache, a_cache, cache_size * sizeof(seq_t));
        free(a_cache);
    }
#else
    /* C++ implementation – seq_t may be a class type that requires
       construction.  Use new[] to invoke constructors.  */
    new_cache = new seq_t[new_size];
    if (new_cache == NULL)
        abort();
    for (size_t i = 0; i < cache_size; ++i)
        new_cache[i] = a_cache[i];
    delete[] a_cache;
#endif

    size_t old_size = cache_size;
    cache_size = new_size;

    /* Initialize newly allocated slots. */
    for (size_t i = old_size; i < new_size; ++i)
        new_cache[i] = 0;

    /* Compute σ(k) and a(k) for all newly required indices. */
    for (idx_t k = (idx_t)old_size; k <= n; ++k) {
        /* σ(k) – sum of divisors */
        seq_t sigma = 0;
        for (idx_t d = 1; d * d <= k; ++d) {
            if (k % d == 0) {
                sigma += (seq_t)d;            /* divisor d */
                idx_t other = k / d;
                if (other != d)
                    sigma += (seq_t)other;     /* complementary divisor k/d */
            }
        }

        if (k == 0) {
            new_cache[0] = 1;                  /* a(0) = 1 */
        } else {
            new_cache[k] = sigma - new_cache[k - 1];  /* recurrence */
        }
    }

    a_cache = new_cache;
}

/* Public entry point: returns a(n).  */
seq_t a(idx_t n)
{
    if ((size_t)n < cache_size)         /* already computed */
        return a_cache[(size_t)n];

    extend_to(n);
    return a_cache[(size_t)n];
}
