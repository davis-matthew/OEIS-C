/*
 * A170621  Number of reduced words of length n in Coxeter group on 36
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^48 = I.
 * https://oeis.org/A170621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A170621 – Number of reduced words of length n in the Coxeter group
 *  on 36 generators with relations  (S_i)^2 = (S_i S_j)^48 = I.
 *
 *  The generating function is
 *        P(t) = 1 + 2t + 2t^2 + … + 2t^47 + t^48
 *        Q(t) = 1 – 34t – 34t^2 – … – 34t^47 + 595t^48
 *
 *  Hence the sequence satisfies the linear recurrence of order 48:
 *
 *      a_n = 34*(a_{n-1}+a_{n-2}+…+a_{n-47}) – 595*a_{n-48},   for n ≥ 48
 *
 *  The first 48 terms are obtained by the recurrence above with the
 *  inhomogeneous part p_n equal to the coefficients of P(t):
 *
 *      p_0 = 1,  p_1 … p_47 = 2,  p_48 = 1,  p_n = 0 (n > 48)
 *
 *  These terms are generated automatically by the code and the values
 *  are stored in a static cache so that subsequent calls can use the
 *  already computed values.
 *
 *  The implementation uses only integer arithmetic; intermediate values
 *  are of type seq_t (which may be arbitrary‑precision) and the constants
 *  34, 595 and the tiny inhomogeneous terms are safely handled.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* Helper to make sure the cache has room for indices up to n. */
static void ensure_cache_capacity(idx_t n, seq_t **cache_ptr, size_t *cap_ptr)
{
    if (*cap_ptr <= (size_t)n) {
        size_t new_cap = (size_t)(n + 1);
        if (new_cap < 64) new_cap = 64;          /* small safe initial size */
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (*cache_ptr) {
            memcpy(new_cache, *cache_ptr, *cap_ptr * sizeof(seq_t));
            free(*cache_ptr);
        }
        *cache_ptr = new_cache;
        *cap_ptr = new_cap;
    }
}

/* Returns the nth term of sequence A170621. */
seq_t a(idx_t n)
{
    /* Static cache of previously computed values. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;      /* allocated size of cache */
    static idx_t cache_len = 0;       /* number of values already computed */

    /* Make sure we have space for at least n+1 entries. */
    ensure_cache_capacity(n, &cache, &cache_cap);

    /* Compute missing terms up to index n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = 0;

        /* Sum 34 * a_{i-k} for k = 1..47 if i >= k. */
        for (idx_t k = 1; k <= 47; ++k) {
            if (i >= k) {
                sum += cache[(size_t)(i - k)] * 34;
            }
        }

        /* Subtract 595 * a_{i-48} when i >= 48. */
        if (i >= 48) {
            sum -= cache[(size_t)(i - 48)] * 595;
        }

        /* Add the inhomogeneous term p_i. */
        seq_t p = 0;
        if (i == 0) {
            p = 1;
        } else if (i == 48) {
            p = 1;
        } else if (i <= 47) {
            p = 2;
        }
        sum += p;

        /* Store the computed value. */
        cache[(size_t)i] = sum;
    }

    cache_len = n + 1;
    return cache[(size_t)n];
}
