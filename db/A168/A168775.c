/*
 * A168775  Number of reduced words of length n in Coxeter group on 50
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^18 = I.
 * https://oeis.org/A168775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A168775: number of reduced words of length n in
   the Coxeter group with 50 generators and relations
   S_i^2 = (S_i S_j)^18 = I.
   The generating function is
      (t^18 + 2*t^17 + … + 2*t + 1)
      --------------------------------
      (1176*t^18 - 48*t^17 - … - 48*t - 48)

   From it we obtain the linear recurrence
      a(0) = 1
      a(n) = 48*Σ_{k=1..min(17,n)} a(n-k) + 2     (1 ≤ n ≤ 17)
      a(18) = 48*Σ_{k=1..17} a(18-k) - 1176*a(0) + 1
      a(n) = 48*Σ_{k=1..17} a(n-k) - 1176*a(n-18)  (n ≥ 19)
   This implementation memoises computed terms.
*/

#include <stdlib.h>

/* Cache for previously computed terms.  */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Ensure the cache is large enough to store terms up to 'upto'. */
static void ensure_cache(idx_t upto)
{
    if (cache_len > upto)
        return;
    idx_t new_len = upto + 1;

    /* Allocate or grow the cache.  This works both in C and C++. */
#ifdef __cplusplus
    seq_t *new_cache = new seq_t[new_len];
    if (cache != NULL) {
        for (idx_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        delete[] cache;
    }
    cache = new_cache;
#else
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache)                 /* allocation failed – leave cache unchanged */
        return;
    if (cache_len > 0) {
        /* initialise newly allocated elements to zero */
        for (idx_t i = cache_len; i < new_len; ++i)
            new_cache[i] = (seq_t)0;
    }
    cache = new_cache;
#endif
    cache_len = new_len;
}

/* Compute a single term a(n) assuming a(0)…a(n‑1) are already in cache. */
static seq_t compute_term(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Σ_{k=1..min(17,n)} a(n-k) */
    seq_t sum = 0;
    idx_t maxk = (n < 17) ? n : 17;
    for (idx_t k = 1; k <= maxk; ++k)
        sum += cache[n - k];

    if (n <= 17) {
        /* a(n) = 48*sum + 2  (c_n = 2 for 1 ≤ n ≤ 17) */
        return (seq_t)48 * sum + (seq_t)2;
    }

    if (n == 18) {
        /* a(18) = 48*sum - 1176*a(0) + 1  (c_18 = 1) */
        return (seq_t)48 * sum - (seq_t)1176 * cache[0] + (seq_t)1;
    }

    /* n ≥ 19: a(n) = 48*sum - 1176*a(n‑18)  */
    return (seq_t)48 * sum - (seq_t)1176 * cache[n - 18];
}

/* Public function required by the specification.  */
seq_t a(idx_t n)
{
    if (n < cache_len)
        return cache[n];

    /* Store the current cache length so that we can compute
       missing terms after the resize. */
    idx_t old_len = cache_len;
    ensure_cache(n);

    for (idx_t i = old_len; i <= n; ++i)
        cache[i] = compute_term(i);

    return cache[n];
}
