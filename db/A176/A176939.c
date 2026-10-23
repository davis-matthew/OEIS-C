/*
 * A176939  Primes of the form 23^n + 10^n.
 * https://oeis.org/A176939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, bref
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Helpers:   is_prime_big, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS: A176939
 * Primes of the form 23^n + 10^n.
 * a(1) = 23^0 + 10^0 = 2,
 * a(2) = 23^7 + 10^7 = 289841,
 * a(3) = 23^12 + 10^12 = 6132620415680998648961, …
 *
 * The function returns the n‑th such prime (n≥1).  If no such prime
 * has been found yet the function continues computing terms until the
 * requested index is reached or until further terms cannot be found.
 *
 * Implementation: iteratively compute 23^k+10^k starting from k=0,
 * test primality with is_prime_big, cache found primes, and
 * return the requested element.  The cache allows calls in any order.
 */

#include <stddef.h>          /* size_t */
#include <stdlib.h>          /* realloc, abort */

#include "../utils/mul_small.h"   /* seq_t mul_small(seq_t, unsigned int) */
#include "../utils/is_prime_big.h"/* bool is_prime_big(seq_t) */

/* Grow a dynamic array to at least 'need' elements. */
static void grow_cache(size_t *cap, seq_t **arr, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap) ? (*cap << 1) : 8;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* unrecoverable, as required */
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Static storage for memoisation. */
static seq_t *cache = NULL; /* cached primes */
static size_t cache_cap = 0;
static idx_t  terms_done = 0; /* number of primes cached */

/* Current powers of 23 and 10 (starting with 23^0 = 10^0 = 1). */
static seq_t pow23 = 1;
static seq_t pow10 = 1;
static idx_t current_exp = 0; /* exponent k */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n = 0 */

    /* Generate terms until we have at least n cached. */
    while (terms_done < n) {
        seq_t candidate = pow23 + pow10; /* 23^k + 10^k for current k */

        if (is_prime_big(candidate)) {
            grow_cache(&cache_cap, &cache, terms_done + 1);
            cache[terms_done] = candidate;
            terms_done++;
        }

        /* Prepare for next exponent. */
        pow23 = mul_small(pow23, 23);
        pow10 = mul_small(pow10, 10);
        current_exp++;
    }

    /* Return the n-th term (1‑based index). */
    return cache[n - 1];
}
