/*
 * A227948  Zeroless numbers n such that n + (product of digits of n) and n -
 *          (product of digits of n) are prime.
 * https://oeis.org/A227948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 1057, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_product, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for zeros‑less numbers n such that
   n + product_of_digits(n) and n – product_of_digits(n) are prime */

/*  The function a(idx_t n) returns the n‑th term of OEIS A227948.
    It caches terms internally so repeated calls are fast. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/digit_product.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* Cached terms of the sequence */
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;   /* number of stored terms          */
    static size_t cache_cap = 0;   /* current allocation capacity     */

    /* Highest integer tested for membership in the sequence */
    static idx_t tested_up_to = 0;

    /* Ensure the cache holds at least n terms */
    while (cache_len < n)
    {
        /* Next candidate integer to test */
        tested_up_to++;

        seq_t s     = (seq_t)tested_up_to;      /* current number            */
        seq_t prod  = digit_product(s);         /* product of its digits     */

        /* Skip numbers containing a zero digit (product zero) */
        if (prod == 0)
            continue;

        /* n - product_of_digits(n) must be ≥ 2 to be prime */
        seq_t n_minus = s - prod;
        if (n_minus < (seq_t)2)
            continue;

        seq_t n_plus = s + prod;
        if (!isPrimeGeneric(n_plus) || !isPrimeGeneric(n_minus))
            continue;     /* ensure both sums are prime */

        /* Store this valid term in the cache */
        if (cache_len + 1 > cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();      /* allocation failure */
            cache     = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = s;
    }

    return cache[n - 1];
}
