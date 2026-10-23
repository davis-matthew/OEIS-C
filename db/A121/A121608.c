/*
 * A121608  Primes that can be written as concatenation of two composite
 *          numbers in decimal representation.
 * https://oeis.org/A121608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4363, u32 n <= 4364, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, is_prime_big, pow10, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A121608 - primes that can be written as concatenation of two composite numbers in decimal.
    The function computes terms by exhaustive search and memoises results.
    For each candidate integer, if it is prime and can be split into two
    decimal parts each >1 that are both composite, it is added to the cache.
    Subsequent calls continue from the last checked value.  */

#include <stdbool.h>
#include <stdlib.h>

/* Utility functions required by the implementation. */
#include "../utils/digit_len.h"   /* int digit_len(seq_t) */
#include "../utils/pow10.h"       /* seq_t pow10(idx_t) */
#include "../utils/is_prime_big.h"/* bool is_prime_big(seq_t) */
#include "../utils/seq_alloc.h"   /* seq_t* seq_alloc(size_t) */
#include "../utils/seq_realloc.h"/* seq_t* seq_realloc(seq_t*, size_t, size_t) */

/* ---------------------------------------------------------------------- */
/*   Helper: check whether a prime can be split into two composite parts  */
static bool is_valid_prime(seq_t p)
{
    int d = digit_len(p);                /* number of decimal digits */
    for (int s = 1; s < d; ++s) {
        seq_t div   = pow10((idx_t)(d - s));      /* 10^(d-s) */
        seq_t left  = p / div;
        seq_t right = p % div;
        if (left > 1 && right > 1 &&          /* both parts > 1       */
            !is_prime_big(left) &&           /* both parts composite*/
            !is_prime_big(right))
            return true;
    }
    return false;
}

/* ---------------------------------------------------------------------- */
/*   Cache management --------------------------------------------------- */
static seq_t *cache      = NULL; /* array holding found primes */
static size_t cache_len  = 0;    /* number of primes stored   */
static size_t cache_cap  = 0;    /* allocated capacity       */
static seq_t last_checked = 2;   /* candidate to test next    */

/* Ensure cache has room for at least `need` elements. */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < need)
        new_cap <<= 1;          /* double until sufficient */

    if (cache == NULL)
        cache = seq_alloc(new_cap);
    else
        cache = seq_realloc(cache, cache_cap, new_cap);
    if (!cache)
        abort();                /* allocation failure */

    cache_cap = new_cap;
}

/* ---------------------------------------------------------------------- */
/*   Public sequence function                                            */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined below offset, harmless */

    size_t target = (size_t)n;      /* desired count of terms */

    while (cache_len < target) {
        /* Test next candidate for primality and validity */
        if (is_prime_big(last_checked) && is_valid_prime(last_checked)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = last_checked;
        }
        last_checked++;           /* advance to next integer */
    }
    return cache[n - 1];
}
