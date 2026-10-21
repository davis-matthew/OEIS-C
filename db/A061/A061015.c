/*
 * A061015  Numerator of Sum_{i=1..n} 1/p(i)^2, p(i) = i-th prime.
 * https://oeis.org/A061015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A061015: Numerator of Sum_{i=1..n} 1/p(i)^2, p(i) = i‑th prime.
   Recurrence: a(1) = 1;
                a(n) = a(n-1)*p(n)^2 + (p(1)*…*p(n-1))^2   (n ≥ 2)   */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_prime.h"

static idx_t    cache_max_n = 0;   /* highest index cached (1‑based) */
static seq_t *  a_cache     = NULL;   /* a[i] for 1‑based indices */
static seq_t *  prod_cache  = NULL;   /* ∏_{j=1}^{i} p(j)^2  */
static size_t  cache_cap   = 0;       /* allocated capacity */

/* Ensure that the cache can hold indices up to 'need'. */
static void ensure_capacity(idx_t need)
{
    if (cache_cap < (size_t)(need + 1))
    {
        size_t new_cap = cache_cap ? cache_cap << 1 : 4;
        while (new_cap < (size_t)(need + 1))
            new_cap <<= 1;

        seq_t *new_a  = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        seq_t *new_p  = (seq_t *)realloc(prod_cache, new_cap * sizeof(seq_t));
        if (!new_a || !new_p) abort();    /* allocation failed */

        a_cache    = new_a;
        prod_cache = new_p;
        cache_cap  = new_cap;
    }
}

/* Main sequence function.  For n == 0 it returns 0 (offset‑independent). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* not part of the sequence */

    if (cache_max_n < n)          /* extend cache up to n */
    {
        ensure_capacity(n);
        for (idx_t i = cache_max_n + 1; i <= n; ++i)
        {
            uint32_t pk   = get_prime(i);          /* i‑th prime as uint32_t */
            seq_t   p     = (seq_t)pk;
            seq_t   p_sq  = p * p;                 /* p(i)^2 */

            if (i == 1)                          /* initial value */
            {
                a_cache[1]    = (seq_t)1;
                prod_cache[1] = p_sq;             /* (p(1))^2 = 4 */
            }
            else
            {
                seq_t prev_a     = a_cache[i - 1];
                seq_t prev_prod  = prod_cache[i - 1];
                a_cache[i]   = prev_a * p_sq + prev_prod;
                prod_cache[i] = prev_prod * p_sq;
            }
        }
        cache_max_n = n;
    }
    return a_cache[n];
}
