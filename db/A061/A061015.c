/*
 * A061015  Numerator of Sum_{i=1..n} 1/p(i)^2, p(i) = i-th prime.
 * https://oeis.org/A061015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 182 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A061015: Numerator of Sum_{i=1..n} 1/p(i)^2, p(i) = i‑th prime.
   Recurrence: a(1) = 1;
                a(n) = a(n-1)*p(n)^2 + (p(1)*…*p(n-1))^2   (n ≥ 2)   */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

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

        seq_t *new_a  = mem_resize(seq_t, a_cache, new_cap);
        seq_t *new_p  = mem_resize(seq_t, prod_cache, new_cap);
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
