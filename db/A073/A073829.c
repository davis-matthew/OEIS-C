/*
 * A073829  a(n) = 4*((n-1)! + 1) + n.
 * https://oeis.org/A073829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*((n-1)! + 1) + n
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *fact_cache = NULL;   /* factorial cache: fact_cache[i] = i! */
static idx_t fact_cache_max = 0;  /* maximum i cached so far */

/* Ensure factorials up to 'up_to' are cached.  Assumes 'up_to' >= 0. */
static void ensure_fact(idx_t up_to)
{
    if (up_to <= fact_cache_max)
        return;

    /* Allocate or grow the cache */
    if (!fact_cache) {
        /* Cache at least up_to+1 elements: index up_to is needed */
        size_t new_size = up_to + 1;
        fact_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!fact_cache) abort();
        fact_cache[0] = (seq_t)1;        /* 0! = 1 */
        fact_cache_max = 0;
    } else {
        size_t old_size = fact_cache_max + 1;
        size_t new_size = up_to + 1;
        if (new_size > old_size) {
            seq_t *tmp = (seq_t *)realloc(fact_cache, new_size * sizeof(seq_t));
            if (!tmp) abort();
            fact_cache = tmp;
        }
    }

    /* Compute missing factorials */
    for (idx_t i = fact_cache_max + 1; i <= up_to; ++i) {
        /* i! = (i-1)! * i */
        fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
    }
    fact_cache_max = up_to;
}

/* Return factorial of n (n ≥ 0).  For n up to current cache size it is exact. */
static seq_t factorial(idx_t n)
{
    if (n == 0) return (seq_t)1;
    ensure_fact(n);
    return fact_cache[n];
}

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 1.  For n==0 we return 0 as placeholder. */
    if (n == 0)
        return (seq_t)0;

    /* Compute (n-1)! */
    seq_t f = factorial(n - 1);

    /* a(n) = 4 * (f + 1) + n = 4*f + 4 + n */
    seq_t result = (f * (seq_t)4) + (seq_t)4 + (seq_t)n;
    return result;
}
