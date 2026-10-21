/*
 * A208209  a(n) = (a(n-1)^2*a(n-2)^2 + 1)/a(n-3) with a(0)=a(1)=a(2)=1.
 * https://oeis.org/A208209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-1)^2 * a(n-2)^2 + 1) / a(n-3) with a(0) = a(1) =
 *            a(2) = 1
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The implementation memoizes already computed values and grows
    the cache geometrically.  It works for both C and C++ compilers,
    whether seq_t is a simple unsigned integer type or an
    arbitrary-precision integer class.  */

#include <stdlib.h>

static seq_t *cache   = NULL;   /* array of already computed terms   */
static idx_t  cap     = 0;      /* current capacity of cache        */
static idx_t  last    = 0;      /* number of terms that are valid   */

static void ensure_cache(idx_t n)
{
    /* Ensure cache can hold indices 0 .. n  (i.e. n+1 elements).  */
    if (n + 1 <= cap) return;

    idx_t new_cap = cap ? cap * 2 : 1;
    while (new_cap <= n) new_cap <<= 1;

#ifdef __cplusplus
    /* In C++ use operator new[] so non‑trivial constructors are
       correctly invoked for an arbitrary‑precision seq_t class. */
    seq_t *new_cache = new seq_t[new_cap];
#else
    /* In C use malloc; seq_t is assumed to be trivially
       constructible (e.g. an unsigned integer type). */
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
    if (!new_cache) abort();  /* fatal if allocation fails */

    for (idx_t i = 0; i < cap; ++i) new_cache[i] = cache[i];

#ifdef __cplusplus
    delete[] cache;
#else
    free(cache);
#endif
    cache = new_cache;
    cap   = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)1;   /* a(0), a(1), a(2) */

    ensure_cache(n);

    /* Initialise the first three terms if they have not yet been
       stored.  */
    if (last < 3) {
        for (idx_t i = last; i < 3; ++i) cache[i] = (seq_t)1;
        last = 3;
    }

    /* Compute all missing terms up to n.  */
    for (idx_t i = last; i <= n; ++i) {
        seq_t t1 = cache[i - 1];
        seq_t t2 = cache[i - 2];
        seq_t t3 = cache[i - 3];

        seq_t prod   = t1 * t1;            /* a(i-1)^2          */
        seq_t t2sq   = t2 * t2;            /* a(i-2)^2          */
        prod        = prod * t2sq;         /* a(i-1)^2*a(i-2)^2*/
        prod        = prod + (seq_t)1;     /* +1                */
        cache[i]    = prod / t3;           /* divide by a(i-3)  */
    }
    last = n + 1;
    return cache[n];
}
