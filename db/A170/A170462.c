/*
 * A170462  Number of reduced words of length n in Coxeter group on 21
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^45 = I.
 * https://oeis.org/A170462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A170462
 *
 *  Definition:
 *      a(0) = 1
 *      a(n) = 21 * 20^(n-1)                                 (1 ≤ n ≤ 44)
 *      a(n) = 19 * Σ_{k=1}^{44} a(n-k) - 190 * a(n-45)      (n ≥ 45)
 *
 *  The function memoises results in a static cache for arbitrary‑precision
 *  seq_t as well as for machine integers.
 */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */
#include <stdlib.h>   /* realloc */

static seq_t *cache = NULL;       /* cache of a(n) values */
static idx_t  cache_len = 0;      /* number of elements stored (≤) */

static void ensure_cache(idx_t n)
{
    if (n < cache_len)                 /* already computed */
        return;

    /* Grow cache: allocate space for indices 0..n (inclusive) */
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    if (!tmp) return;                  /* allocation failed – keep returning 0 */
    cache = tmp;
    cache_len = n + 1;

    /* (1) Initialise a(0) */
    cache[0] = (seq_t)1;

    /* (2) Initialise a(1)…a(min(44, n)) by the explicit formula */
    if (n >= 1) {
        seq_t cur = (seq_t)1;               /* 20^(i-1) for i=1 */
        idx_t limit = n < 44ULL ? (idx_t)n : 44ULL;
        for (idx_t i = 1; i <= limit; ++i) {
            cache[i] = (seq_t)21 * cur;
            cur *= (seq_t)20;
        }
    }

    /* (3) For n ≥ 45 compute terms using the recurrence */
    if (n < 45ULL)
        return;                                      /* nothing more to do */

    /* Compute the sum of the first 44 values (a[1] … a[44]) */
    seq_t sum_prev44 = (seq_t)0;
    for (idx_t i = 1; i <= 44 && i < cache_len; ++i)
        sum_prev44 += cache[i];

    /* Compute a[45] … a[n] */
    for (idx_t i = 45; i < cache_len; ++i) {
        seq_t term = (seq_t)19 * sum_prev44 - (seq_t)190 * cache[i - 45];
        cache[i] = term;

        /* Update sliding window sum for the next iteration */
        sum_prev44 += cache[i];
        sum_prev44 -= cache[i - 44];
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    if (n < cache_len)
        return cache[n];
    return (seq_t)0;      /* in case of allocation failure */
}
