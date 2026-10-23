/*
 * A119394  a(n) = Sum_{k=0..n} (-1)^(n-k)*(n!/k!)^2*binomial(n-1,k-1).
 * https://oeis.org/A119394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 8, i64 n <= 12, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A119394
 * Formula: a(n) = Sum_{k=0..n} (-1)^(n-k) * ((n!/k!)^2) * binomial(n-1,k-1)
 * Recurrence: a(0)=1, a(1)=1, a(2)=-3
 * a(n) = -(3*n^2-5*n+1)*a(n-1)
 *          -(n-2)*(n-1)^2*(3*n-4)*a(n-2)
 *          -(n-3)*(n-2)^3*(n-1)^2*a(n-3)
 */
#include <stdlib.h>

/* External integer types supplied by the build environment */
extern seq_t a(idx_t n);

/* Cache for previously computed values.  The cache is resized
   lazily and grows geometrically.  The pointer type seq_t* is
   compatible with the seq_t type used by the test harness. */
static seq_t *cache = NULL;     /* array of computed terms */
static size_t cache_cap = 0;    /* capacity in terms */
static size_t cache_len = 0;    /* number of terms actually computed */

/* Ensure that the cache can hold terms up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    if (new_cap < needed)
        new_cap = needed;

    /* In C++ a cast from void* to a class pointer is required.  A
       C-style cast works in both C and C++.  The build environment
       guarantees that realloc and calloc return heap memory suitable
       for seq_t objects. */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Public function required by the test harness.  It computes a(n)
   using the recurrence above, stores the result in a cache so that
   subsequent calls with the same or larger n are O(n).  Calls with
   smaller n are answered directly from the cache. */
seq_t a(idx_t n)
{
    /* Allocate cache lazily on the first call. */
    if (cache == NULL)
    {
        cache_cap = 4; /* at least space for the first three values */
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        /* Initialise the known values. */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)-3;
        cache_len = 3;
    }

    /* Expand the cache if necessary. */
    ensure_cache(n);

    size_t idx = (size_t)n;
    /* Compute missing terms up to idx. */
    for (size_t i = cache_len; i <= idx; ++i)
    {
        if (i < 3)
        {
            /* indices 0,1,2 already initialised */
            continue;
        }

        seq_t xi = (seq_t)i;

        /* Retrieve the three previous terms. */
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];

        /* Coefficient for a(i-1). */
        seq_t coeff1 = xi * xi * (seq_t)3 - xi * (seq_t)5 + (seq_t)1;
        seq_t part1  = -(coeff1 * a1);

        /* Coefficient for a(i-2). */
        seq_t coeff2 = (xi - (seq_t)2) * (xi - (seq_t)1) *
                       (xi - (seq_t)1) *
                       ((seq_t)3 * xi - (seq_t)4);
        seq_t part2  = -(coeff2 * a2);

        /* Coefficient for a(i-3). */
        seq_t coeff3 = (xi - (seq_t)3) *
                       (xi - (seq_t)2) *
                       (xi - (seq_t)2) *
                       (xi - (seq_t)2) *
                       (xi - (seq_t)1) *
                       (xi - (seq_t)1);
        seq_t part3  = -(coeff3 * a3);

        cache[i] = part1 + part2 + part3;
    }

    cache_len = idx + 1;
    return cache[idx];
}
