/*
 * A230226  Odd odious numbers (A000069) which can be written as a product of
 *          two odious numbers > 1.
 * https://oeis.org/A230226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th odd integer that can be expressed as a product
 *            of two odious numbers greater than 1. An odious number has an
 *            odd number of 1-bits in its binary representation
 * Range:     u8 n <= 1, u16 n <= 1021, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard utilities needed for the routine. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Check if an integer has an odd number of 1‑bits (odious). */
static bool is_odious(seq_t x)
{
    unsigned int parity = 0;
    seq_t t = x;
    while (t) {
        parity ^= (unsigned int)(t & 1);
        t >>= 1;
    }
    return parity != 0;
}

/* Return true if x can be expressed as a product of two odious numbers > 1.
   Only odd divisors need be examined; the loop stops at sqrt(x). */
static bool has_odious_product(seq_t x)
{
    seq_t limit = floor_sqrt(x);
    for (seq_t i = 3; i <= limit; i += 2) {
        if (x % i == 0) {
            seq_t j = x / i;
            if (i > 1 && j > 1 && is_odious(i) && is_odious(j))
                return true;
        }
    }
    return false;
}

/* Return the n-th term of the sequence (n ≥ 1).  The result is cached
   to allow out-of‑order requests. */
seq_t a(idx_t n)
{
    /* Static tables for memoisation */
    static seq_t *cache = NULL;   /* cached terms */
    static size_t cap   = 0;      /* capacity of cache */
    static size_t len   = 0;      /* number of terms stored */
    static seq_t next_candidate = 3;   /* smallest odd > 1 to test */

    /* Expand the cache as needed */
    while (len < n) {
        seq_t cand = next_candidate;

        /* Ensure we examine only odd numbers */
        while ((cand & 1) == 0) cand++;

        /* Search for the next qualifying term */
        for (;;) {
            if (is_odious(cand) && has_odious_product(cand)) {
                /* Store the term */
                if (len + 1 > cap) {
                    size_t new_cap = cap ? cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) {
                        /* Allocation failed – abort gracefully */
                        return (seq_t)0;
                    }
                    cache = tmp;
                    cap   = new_cap;
                }
                cache[len++] = cand;
                next_candidate = cand + 2;   /* next odd to test */
                break;
            }
            cand += 2;   /* skip to the following odd */
        }
    }

    return cache[n - 1];
}
