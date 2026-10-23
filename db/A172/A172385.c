/*
 * A172385  a(n) = 1 if n=0, otherwise Sum_{k=0..floor((n-1)/2)}
 *          binomial(n-k-1, k) *(-1)^k*a(n-1-2k).
 * https://oeis.org/A172385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 1 if n=0, else ∑_{k=0..⌊(n-1)/2⌋}
 *            C(n-k-1,k)·(-1)^k·a(n-1-2k)
 * Range:     i8 n <= 9, i16 n <= 15, i32 n <= 25, i64 n <= 41, i128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* Static cache for memoised values */
static seq_t *cache = NULL;          /* array of computed terms   */
static idx_t cache_len = 0;          /* current number of elements stored  */
static idx_t cache_cap = 0;          /* capacity of allocated array        */

/* Ensure the cache can hold at least 'needed' elements */
static void ensure_capacity(idx_t needed)
{
    if (cache_cap >= needed)
        return;

    /* Find new capacity (at least double the old) */
    idx_t new_cap = cache_cap == 0 ? 1 : cache_cap;
    while (new_cap < needed)
        new_cap <<= 1;   /* multiply by 2 */

    if (cache_cap == 0) {
#ifdef __cplusplus
        cache = (seq_t*)::operator new((size_t)new_cap * sizeof(seq_t));
#else
        cache = (seq_t*)malloc((size_t)new_cap * sizeof(seq_t));
#endif
        cache_cap = new_cap;
        return;
    }

#ifdef __cplusplus
    /* Allocate new block */
    seq_t *new_cache = (seq_t*)::operator new((size_t)new_cap * sizeof(seq_t));

    /* Copy/construct existing elements into new block */
    for (idx_t i = 0; i < cache_len; ++i) {
        ::new(&new_cache[i]) seq_t(cache[i]);   /* copy‑construct */
    }

    /* Destroy old objects */
    for (idx_t i = 0; i < cache_len; ++i) {
        cache[i].~seq_t();
    }

    /* Deallocate old memory */
    ::operator delete(cache);

    cache = new_cache;
#else
    cache = (seq_t*)realloc(cache, (size_t)new_cap * sizeof(seq_t));
#endif
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Initialise cache with a(0) = 1 if necessary */
    if (cache_len == 0) {
        ensure_capacity(1);
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Compute missing values up to index n */
    for (idx_t m = cache_len; m <= n; ++m) {
        idx_t maxk = (m - 1) / 2;
        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;            /* C(m-1,0) */

        for (idx_t k = 0; k <= maxk; ++k) {
            seq_t term = cache[m - 1 - 2 * k];
            term = binom * term;
            if (k & 1) term = -term;       /* (-1)^k factor */
            sum += term;

            if (k < maxk) {
                /* Update binomial coefficient to C(m-k-2, k+1) */
                seq_t num1   = (seq_t)(m - 2 * k - 1);
                seq_t num2   = (seq_t)(m - 2 * k - 2);
                seq_t denom1 = (seq_t)(m - k - 1);
                seq_t denom2 = (seq_t)(k + 1);
                binom = (binom * num1 * num2) / (denom1 * denom2);
            }
        }

        ensure_capacity(cache_len + 1);
        cache[cache_len] = sum;
        ++cache_len;
    }

    return cache[n];
}
