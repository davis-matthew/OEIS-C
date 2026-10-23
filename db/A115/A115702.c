/*
 * A115702  Triangular numbers whose digit reversal is the product of 2
 *          palindromes greater than 1.
 * https://oeis.org/A115702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 27, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   floor_sqrt, is_palindrome, rev_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A115702: A number is a triangular number T(n)=n*(n+1)/2
   such that the decimal reversal of T(n) can be factored into two
   palindromic integers greater than 1. */
#include <stddef.h>      /* for size_t */
#include "../utils/rev_decimal.h"   /* rev_decimal(seq_t)            */
#include "../utils/floor_sqrt.h"    /* floor_sqrt(seq_t)             */
#include "../utils/is_palindrome.h" /* is_palindrome(seq_t, idx_t)   */

/* static state for memoisation ---------------------------------------- */
static idx_t      next_k = 1;        /* next triangular index to test   */
static seq_t     *cache  = NULL;     /* array of found terms             */
static size_t     cache_len = 0;     /* number of terms already cached   */
static size_t     cache_cap  = 0;     /* allocated capacity of cache      */

/* enlarge the cache when necessary ----------------------------------- */
static void
ensure_cache_cap(void)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* test whether revT can be written as a product of two palindromes > 1
   (base 10).  The search is sequential over possible divisors up to
   sqrt(revT). */
static bool
qualifies(seq_t revT)
{
    seq_t sqrt_rev = floor_sqrt(revT);           /* integer sqrt            */

    for (seq_t p = 2; p <= sqrt_rev; ++p) {
        if (!is_palindrome(p, 10))            /* skip non‑palindromes    */
            continue;

        if (revT % p == 0) {
            seq_t q = revT / p;
            if (q > 1 && is_palindrome(q, 10))
                return true;
        }
    }
    return false;
}

/* main sequence function -------------------------------------------- */
seq_t
a(idx_t n)
{
    /* generate terms until the requested one is cached */
    while (cache_len < n) {
        seq_t k  = (seq_t)next_k;
        seq_t T  = k * (k + 1) / 2;            /* triangular number       */
        seq_t revT = rev_decimal(T);           /* reverse(T) in base 10   */

        if (qualifies(revT)) {
            ensure_cache_cap();
            cache[cache_len++] = T;
        }
        ++next_k;
    }

    return cache[n - 1];   /* OEIS indices are 1‑based */
}
