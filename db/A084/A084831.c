/*
 * A084831  Numbers k such that sum of odd divisors and sum of even divisors
 *          are both palindromic.
 * https://oeis.org/A084831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 437, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_pal_uint128, sum_odd_even
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A084831:  numbers k such that the sum of odd divisors of k and the
   sum of even divisors of k are both palindromic in base 10.  The
   routine enumerates successive integers, checks the required
   property, and caches the found terms.  It uses a 128‑bit integer
   for intermediate calculations so that the palindrome test is
   correct even when the divisor sums exceed 64 bits.  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_pal_uint128.h"
#include "../utils/sum_odd_even.h"

/* ---------- cache for the sequence ---------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t test_k = 1;           /* next integer to test */

/* ---------- main function required by the build ---------- */
seq_t a(idx_t n)
{
    /* Expand the cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        unsigned __int128 odd_sum, even_sum;
        sum_odd_even(test_k, &odd_sum, &even_sum);

        if (is_pal_uint128(odd_sum) && is_pal_uint128(even_sum)) {
            /* Allocate or grow the cache if necessary. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    free(cache);
                    cache = NULL;
                    cache_len = 0;
                    cache_cap = 0;
                    return (seq_t)0;    /* memory failure -> safe error value */
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)test_k;
        }
        ++test_k;
    }
    return cache[(size_t)n - 1];
}
