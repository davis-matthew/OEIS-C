/*
 * A081652  Greatest common divisor of n and sum of decimal digits of n-th
 *          prime.
 * https://oeis.org/A081652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = GCD( n , sum_of_decimal_digits( nth_prime(n) ) )
 * Range:     u8 n <= 55, u16 n <= 6697, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 120 terms through n = 9065 (2026-11-03)
 * Helpers:   digit_sum, gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence offset: 1 (a(1) is the first term) */

#include <stddef.h>                 /* for size_t */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/gcd.h"
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

/* Compute the n‑th term of A081652.
   For n == 0 (below the offset) the function returns 0.  */
seq_t a(idx_t n)
{
    /* Return zero for indices below the offset. */
    if (n == 0) return 0;

    /* Static cache for previously computed terms.
       The cache is sized to hold at least n terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* Ensure sufficient cache capacity. */
    ensure_cache(n, &cache, &cache_cap);

    /* Return a cached value if available. */
    if (cache[n - 1] != 0) return cache[n - 1];

    /* Compute the n‑th prime (prime_1 = 2). */
    uint32_t prime32 = get_prime(n);          /* returns uint32_t */
    seq_t prime_val  = (seq_t)prime32;        /* safe cast – prime < 2^32 for the ranges used */

    /* Sum of decimal digits of the prime. */
    seq_t digit_sum_val = digit_sum(prime_val);

    /* GCD of n and the digit sum.  Cast n to seq_t first. */
    seq_t result = gcd((seq_t)n, digit_sum_val);

    /* Store the computed value in the cache. */
    cache[n - 1] = result;

    return result;
}
