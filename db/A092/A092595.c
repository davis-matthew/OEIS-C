/*
 * A092595  Numbers k such that the sum of decimal digits of k and k+1 are
 *          both prime numbers, i.e., both k and k+1 are in A028834.
 * https://oeis.org/A092595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 578, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _digit_sum, _is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n‑th integer k (k ≥ 2) such that digitSum(k) and
 *              digitSum(k+1) are both prime numbers.
 * This implementation searches sequentially and keeps a cache of found
 * terms so that arbitrary calls to a(idx_t n) are answered in O(1) after
 * the first search.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_digit_sum.h"   /* seq_t _digit_sum(seq_t v); */
#include "../utils/_is_prime.h"    /* bool _is_prime(uint64_t n); */

/* static cache state -----------------------------------------------------*/
static seq_t *cache    = NULL;      /* terms found so far           */
static size_t cache_len = 0;        /* number of elements stored    */
static size_t cache_cap = 0;        /* capacity of the array        */
static idx_t  last_checked = 0;     /* last integer examined        */

/* enlarge cache when needed ---------------------------------------------*/
static void grow_cache(void)
{
    size_t new_cap = (cache_cap ? cache_cap * 2 : 8);
    seq_t *ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!ptr) abort();                 /* allocation failed */
    cache = ptr;
    cache_cap = new_cap;
}

/* sequence function ------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset is 1, n==0 is unused */

    /* produce terms until we have at least n cached elements */
    while (cache_len < (size_t)n) {
        ++last_checked;                /* next integer to test   */

        if (last_checked < 2)          /* k must be >= 2          */
            continue;

        /* check digit sums using helper routines */
        if (!_is_prime((uint64_t)_digit_sum((seq_t)last_checked)))
            continue;

        if (!_is_prime((uint64_t)_digit_sum((seq_t)(last_checked + 1))))
            continue;

        /* store the found term */
        if (cache_len == cache_cap)
            grow_cache();
        cache[cache_len++] = (seq_t)last_checked;
    }

    return cache[n - 1];               /* a(1) is first element  */
}
