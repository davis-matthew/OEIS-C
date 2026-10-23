/*
 * A125663  Numbers such that the left half of the digits form a prime and the
 *          right half do not.
 * https://oeis.org/A125663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   digits_of, is_prime_big, seq_alloc_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the n‑th positive integer whose left half of digits
 * (floor(#digits/2) leading digits) is a prime and whose right half
 * (floor(#digits/2) trailing digits, ignoring the middle digit when the
 * number of digits is odd) is NOT a prime.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digits_of.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"   /* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) */
#include "../utils/seq_alloc_array.h" /* seq_alloc_array(size_t n) */

/* Compute 10^exp using the index type (unsigned 64‑bit). */
static idx_t pow10_idx(int exp)
{
    idx_t r = 1;
    for (int i = 0; i < exp; ++i)
        r *= 10;
    return r;
}

/* Return the n-th term of OEIS sequence A125663. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for indices < 1 */

    /* Persistent cache of found terms. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;

    /* Next candidate to examine (first known term is 20). */
    static idx_t cand   = 20;

    /* Scan until we have at least n terms cached. */
    while (len < n) {
        /* Number of decimal digits of candidate. */
        int   d        = digits_of(cand);   /* floor(log10(cand)) + 1 */
        int   half     = d / 2;             /* floor(d/2) */
        idx_t pow_half = pow10_idx(half);          /* 10^half */
        idx_t pow_left = pow10_idx(d - half);      /* 10^(d-half) */

        /* Extract leftmost and rightmost halves. */
        idx_t left  = cand / pow_left;          /* first 'half' digits */
        idx_t right = cand % pow_half;          /* last  'half' digits */

        /* Check the property using the prime test. */
        if (is_prime_big((seq_t)left) && !is_prime_big((seq_t)right)) {
            /* Grow cache if needed. */
            if (len == cap) {
                size_t new_cap = (cap == 0) ? 64 : (cap << 1);
                seq_t *tmp;
                if (cap == 0)
                    tmp = seq_alloc_array(new_cap);
                else
                    tmp = seq_realloc(cache, cap, new_cap);
                if (!tmp)                /* allocation failure */
                    return 0;
                cache = tmp;
                cap   = new_cap;
            }
            /* Store the term.  The conversion (seq_t)cand is valid for
             * all supported seq_t types (integer or arbitrary‑precision). */
            cache[len++] = (seq_t)cand;
        }

        ++cand;      /* move to next integer */
    }

    return cache[n - 1];
}
