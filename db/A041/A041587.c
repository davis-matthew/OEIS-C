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
 * Validated: standard, pass, 46 terms through n = 190 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

/* compute the first 32 partial quotients of the continued fraction for sqrt(311) */
static void fill_partial_quotients(seq_t cf[32])
{
    const seq_t D   = 311;    /* D is not a perfect square */
    const seq_t a0  = 17;     /* floor(sqrt(311)) */
    seq_t m = 0;
    seq_t d = 1;
    seq_t a = a0;

    cf[0] = a0;
    for (size_t i = 1; i < 32; ++i)
    {
        seq_t m_next = d * a - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        cf[i] = a_next;
        m = m_next;
        d = d_next;
        a = a_next;
    }
}

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
