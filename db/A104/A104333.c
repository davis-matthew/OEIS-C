/*
 * A104333  Primes which are the reverse concatenation of four consecutive odd
 *          numbers.
 * https://oeis.org/A104333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 7, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   digit_len, is_prime_seq, pow10_idx, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A104333: primes equal to the reverse concatenation of four consecutive odd numbers.
   The function searches for the next prime by testing candidates of the form
   concat(x, x-2, x-4, x-6) where x is odd, stopping when the desired index n is reached. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/digit_len.h"      /* idx_t digit_len(seq_t x); */
#include "../utils/pow10_idx.h"      /* seq_t pow10_idx(idx_t exp); */
#include "../utils/is_prime_seq.h"   /* bool is_prime_seq(seq_t x); */
#include "../utils/seq_realloc.h"    /* seq_t* seq_realloc(seq_t* old, idx_t new_size); */

seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *cache      = NULL;  /* array of primes */
    static idx_t   cache_sz  = 0;     /* number of stored terms */
    static idx_t   cache_cap = 0;     /* allocated capacity */

    /* Current odd number to test (start from 7: 3,5,7,9 are the first four odds). */
    static seq_t next_x = 7;

    if (n == 0) return 0;            /* undefined for offset 0 */

    /* Grow cache until it contains at least n terms. */
    while (cache_sz < n) {
        seq_t cand = 0;

        /* Find the next prime of the required form. */
        while (1) {
            seq_t a_val = next_x;
            seq_t b_val = next_x - 2;
            seq_t c_val = next_x - 4;
            seq_t d_val = next_x - 6;

            /* Build the reverse concatenation as a seq_t. */
            cand = 0;
            cand = cand * pow10_idx(digit_len(a_val)) + a_val;
            cand = cand * pow10_idx(digit_len(b_val)) + b_val;
            cand = cand * pow10_idx(digit_len(c_val)) + c_val;
            cand = cand * pow10_idx(digit_len(d_val)) + d_val;

            if (is_prime_seq(cand)) break;   /* found the next prime */

            next_x += 2;   /* skip to the next odd number */
        }

        /* Ensure we have enough capacity in the cache. */
        if (cache_sz == cache_cap) {
            cache_cap = cache_cap ? cache_cap * 2 : 8;
            cache = seq_realloc(cache, cache_cap);
        }

        cache[cache_sz++] = cand;   /* store the found prime */
        next_x += 2;                /* prepare for the next search */
    }

    return cache[n - 1];  /* cache is 0‑indexed, while OEIS indices start at 1 */
}
