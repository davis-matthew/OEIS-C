/*
 * A052856  E.g.f.: (1-3*exp(x)+exp(2*x))/(exp(x)-2).
 * https://oeis.org/A052856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1 For n ≥ 1: a(n) = Σ_{k=0}^{n-1} binom(n,k) · a(k) + 3 –
 *            2^n
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation keeps a cache of previously computed terms.  */
#include <stdlib.h>   /* for malloc, realloc, realloc */
#include <stddef.h>   /* for size_t */

/* Cache data */
static seq_t *a_cache      = NULL;   /* cached a(n) values             */
static seq_t *pow2_cache   = NULL;   /* cached 2^n values              */
static size_t cache_cap    = 0;      /* current size of the cache      */
static idx_t  max_computed = 0;      /* largest n that has been computed */

/* Ensure the cache can hold indices 0..n.  */
static void ensure_cache_size(idx_t n)
{
    size_t needed = (size_t)(n + 1);
    if (needed == 0)
        return;
    if (needed <= cache_cap)
        return;
    /* Allocate or grow the arrays.  C++ requires an explicit cast.  */
    a_cache = (seq_t *)realloc(a_cache, needed * sizeof(*a_cache));
    pow2_cache = (seq_t *)realloc(pow2_cache, needed * sizeof(*pow2_cache));
    /* If realloc failed, pointers become NULL; the build framework
       will treat that as an error.  */
    cache_cap = needed;
}

/* Public function computing a(n).  */
seq_t a(idx_t n)
{
    /* Initialise the cache on first use.  */
    if (cache_cap == 0) {
        ensure_cache_size(n);
        a_cache[0]    = (seq_t)1;
        pow2_cache[0] = (seq_t)1;          /* 2^0 = 1 */
        max_computed  = 0;
    }

    /* If n is already computed, return it.  */
    if ((idx_t)n <= max_computed)
        return a_cache[n];

    /* Compute all missing terms up to n.  */
    ensure_cache_size(n);
    for (size_t i = (size_t)(max_computed + 1); i <= (size_t)n; ++i) {
        /* Update 2^i.  */
        pow2_cache[i] = pow2_cache[i - 1] * (seq_t)2;

        /* Compute Σ_{k=0}^{i-1} binom(i,k) · a(k).  */
        seq_t sum   = 0;
        seq_t binom = 1;   /* binom(i,0) */
        for (size_t k = 0; k < i; ++k) {
            sum += binom * a_cache[k];
            /* binom(i,k+1) = binom(i,k) * (i - k) / (k + 1)  */
            binom = binom * (seq_t)(i - k) / (seq_t)(k + 1);
        }

        /* Apply the recurrence.  */
        a_cache[i] = sum + (seq_t)3 - pow2_cache[i];
    }

    max_computed = n;
    return a_cache[n];
}
