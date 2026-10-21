/*
 * A052043  Squares of primes lacking the digit zero in their decimal
 *          expansion.
 * https://oeis.org/A052043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_zero_digit_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence: a(n) = (p_n)^2 where p_n is the n-th prime such that
   the square contains no digit zero in its decimal representation.
   This corresponds to OEIS A052043. 
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/has_zero_digit_seq.h"

/* dynamic cache for the sequence */
static seq_t *cache      = NULL;   /* cached squares */
static size_t cache_cap  = 0;      /* allocated capacity */
static size_t cache_len  = 0;      /* number of stored terms */
static idx_t  last_checked = 1;    /* last integer checked for primality */

/* -------------------------------------------------------------
   Returns the n-th term of A052043 (1‑based index). 
   -------------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; behaviour for n<1 is irrelevant */

    /* generate terms until the cache contains at least n items */
    while (cache_len < n) {
        ++last_checked;

        if (!is_prime(last_checked)) continue;

        seq_t sq = (seq_t)last_checked * (seq_t)last_checked;
        if (has_zero_digit_seq(sq)) continue;

        if (cache_len == cache_cap) {
            cache_cap = (cache_cap < 4) ? 4 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            cache = tmp;
        }
        cache[cache_len++] = sq;
    }

    return cache[n - 1];
}
