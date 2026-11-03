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
 * Validated: standard, pass, 74 terms through n = 9469 (2026-11-03)
 * Helpers:   digit_product, is_prime_u128, is_prime_u64, mem
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

#include <limits.h>
#include "../utils/digit_product.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

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
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) abort();      /* allocation failure */
            cache     = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = s;
    }

    return cache[n - 1];
}
