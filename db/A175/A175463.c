/*
 * A175463  Numbers k such that 8*k + 5 is semiprime.
 * https://oeis.org/A175463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  Compute A175463: the n‑th integer k such that 8*k+5 is a semiprime
 *  (a product of exactly two primes, counting multiplicity).
 *  The sequence is indexed with offset 1, so a(1)=2.
 *
 *  Implementation strategy:
 *  • Keep a static cache of the terms that have been found so far.
 *  • Extend the cache by scanning k=1,2,3… and testing whether
 *    8*k+5 has exactly two prime factors (using the supplied helper).
 *  • Once the cache contains the n‑th term, return it.
 *
 *  The code works for any integer type supplied by the build
 *  (seq_t, idx_t), and uses only integer arithmetic.
 */

#include <stddef.h>               /* size_t */
#include <stdlib.h>               /* malloc, realloc, abort */

#include "../utils/count_prime_factors.h"   /* count_prime_factors(seq_t) */

/* ------------------------------------------------------------------ */
/*            static cache for the sequence values                    */
/* ------------------------------------------------------------------ */
static seq_t *cache = NULL;      /* cached a(i) values */
static size_t cache_len = 0;     /* number of values currently cached */
static size_t cache_cap = 0;     /* allocated size of the cache        */

/* Append a new term to the cache, growing it if necessary */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 256 : cache_cap << 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/*  ------------------------------------------------------------------ */
/*                public sequence routine                            */
/*  ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* OEIS offset: a(1) is the first term.  n==0 is outside the range,
       but we return 0 for safety. */
    if (n == 0) return 0;

    /* If the term is already cached, return it directly. */
    if (cache_len >= (size_t)n) {
        return cache[n - 1];
    }

    /* Start searching from 1 or the next integer after the last cached
       term (converted to idx_t). */
    idx_t k = (cache_len == 0) ? 1 : (idx_t)(cache[cache_len - 1] + 1);

    for (;; ++k) {
        /* Compute m = 8*k + 5 using idx_t to avoid overflow until cast. */
        idx_t m64 = 8 * k + 5;
        seq_t m = (seq_t)m64;      /* cast to the build's seq_t type */

        /* Test whether m is a semiprime: exactly two prime factors (with
           multiplicity).  The helper returns a size_t. */
        if (count_prime_factors(m) == 2) {
            cache_append(k);           /* store the k value as the term */
            if (cache_len >= (size_t)n) {
                return cache[n - 1];   /* the required term is ready */
            }
        }
    }
}
