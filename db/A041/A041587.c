/*
 * A041587  Denominators of continued fraction convergents to sqrt(311).
 * https://oeis.org/A041587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = the n-th denominator of the convergents to sqrt(311) a(n)
 *            = 33767760*a(n-16) - a(n-32) for n > 31 initial values
 *            a(0)..a(15) are given in the OEIS entry
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 22, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   ensure_cache, fill_partial_quotients
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/ensure_cache.h"
#include "../utils/fill_partial_quotients.h"

/* compute and store the first 32 denominators of convergents in cache[0..31] */
static void compute_initial_terms(seq_t *cache)
{
    seq_t cf[32];
    fill_partial_quotients(cf);

    seq_t p_prev2 = 0; /* p_-2 */
    seq_t p_prev1 = 1; /* p_-1 */
    seq_t q_prev2 = 1; /* q_-2 */
    seq_t q_prev1 = 0; /* q_-1 */

    for (size_t k = 0; k < 32; ++k)
    {
        seq_t a_val = cf[k];
        seq_t p_k = a_val * p_prev1 + p_prev2;
        seq_t q_k = a_val * q_prev1 + q_prev2;

        cache[k] = q_k;

        p_prev2 = p_prev1;  p_prev1 = p_k;
        q_prev2 = q_prev1;  q_prev1 = q_k;
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;  /* number of terms already computed */
    static bool   initialized = false;

    /* Ensure the cache can hold at least max(n,31)+1 elements,
       because we need the first 32 terms for the recurrence. */
    idx_t need = n > 31 ? n : 31;
    ensure_cache(need, &cache, &cache_cap);

    /* Compute the first 32 terms once. */
    if (!initialized)
    {
        compute_initial_terms(cache);
        cache_len = 32;
        initialized = true;
    }

    /* Compute any missing terms up to n. */
    if ((size_t)n >= cache_len)
    {
        /* If the requested index is larger than the current capacity,
           enlarge the cache. */
        if ((size_t)n + 1 > cache_cap)
            ensure_cache(n, &cache, &cache_cap);

        for (size_t idx = cache_len; idx <= (size_t)n; ++idx)
        {
            cache[idx] = (seq_t)33767760 * cache[idx-16] - cache[idx-32];
        }
        cache_len = (size_t)n + 1;
    }

    return cache[n];
}
