/*
 * A068100  a(1) = 1; a(n+1) = n!*Sum_{k|n} a(k)/k!.
 * https://oeis.org/A068100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n+1) = n!*∑_{k|n} a(k)/k! (OEIS A068100)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 25,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>            /* size_t */
#include <stdlib.h>            /* free */

/* Utilities for dynamic array allocation */
#include "../utils/alloc_seq_array.h"   /* seq_t *alloc_seq_array(size_t n); */
#include "../utils/free_seq_array.h"    /* void free_seq_array(seq_t *p); */

static seq_t *a_cache   = NULL;   /* cached a(n) values; index 0 unused */
static seq_t *fact_cache = NULL;  /* cached factorials n!; index 0 = 1 */
static size_t cache_cap = 0;      /* current capacity of a_cache and fact_cache */
static size_t fact_max  = 0;      /* largest n for which fact_cache[n] is known */

/* Ensure that the caches can hold indices up to n (inclusive). */
static void
ensure_capacity(idx_t n)
{
    if ((size_t)n < cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;   /* double until large enough */

    seq_t *new_a    = alloc_seq_array(new_cap);
    seq_t *new_fact = alloc_seq_array(new_cap);

    /* copy existing contents */
    for (size_t i = 0; i < cache_cap; ++i) {
        new_a[i]    = a_cache[i];
        new_fact[i] = fact_cache[i];
    }

    free_seq_array(a_cache);
    free_seq_array(fact_cache);

    a_cache   = new_a;
    fact_cache = new_fact;
    cache_cap  = new_cap;

    /* initialise factorial[0] = 1 if it hasn't been set yet */
    if (cache_cap > 0 && fact_cache[0] == 0)
        fact_cache[0] = (seq_t)1;

    /* compute missing factorials up to n */
    if (fact_max < (size_t)n) {
        for (size_t i = fact_max + 1; i <= (size_t)n; ++i)
            fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
        fact_max = (size_t)n;
    }
}

/* Returns a(n) for any n ≥ 1. Uncached values are computed recursively. */
seq_t a(idx_t n)
{
    if (n == 0)                     /* undefined for offset 1, but safe */
        return (seq_t)0;

    ensure_capacity(n);

    /* Base case */
    if (n == 1) {
        if (a_cache[1] == 0)
            a_cache[1] = (seq_t)1;
        return a_cache[1];
    }

    /* Return memoised value if already computed */
    if (a_cache[n] != 0)
        return a_cache[n];

    idx_t m = n - 1;          /* we need divisors of m = n-1 */
    seq_t sum = 0;

    /* Ensure factorial[m] is available */
    if (fact_max < (size_t)m) {
        for (size_t i = fact_max + 1; i <= (size_t)m; ++i)
            fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
        fact_max = (size_t)m;
    }

    /* Iterate over all positive divisors of m */
    for (idx_t d = 1; d * d <= m; ++d) {
        if (m % d == 0) {
            /* First divisor d */
            seq_t val_d = a(d);
            seq_t term  = val_d * (fact_cache[m] / fact_cache[d]);
            sum += term;

            idx_t e = m / d;
            if (e != d) {                    /* second divisor m/d if distinct */
                seq_t val_e = a(e);
                seq_t term2 = val_e * (fact_cache[m] / fact_cache[e]);
                sum += term2;
            }
        }
    }

    a_cache[n] = sum;
    return sum;
}
