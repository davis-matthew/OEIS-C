/*
 * A174569  Numbers n such that sum of factorials of digits of n is a power of
 *          2.
 * https://oeis.org/A174569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A174569: numbers n such that the sum of factorials of
   the decimal digits of n is a power of 2.  The function searches the
   integers sequentially, stores every found term in a cache, and returns
   the nth term (1‑based).  The result is correct for any n ≥ 1 and can be
   called in any order. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_power_of_two.h"

/* factorials of the decimal digits 0–9 */
static const seq_t fact[10] = {
    (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)6,
    (seq_t)24, (seq_t)120, (seq_t)720, (seq_t)5040,
    (seq_t)40320, (seq_t)362880
};

/* cache of discovered terms: cache[0] holds a(1) */
static seq_t *cache = NULL;          /* pointer to array of seq_t */
static size_t cache_len = 0;         /* number of terms stored so far */
static size_t cache_cap = 0;         /* allocated capacity of cache */
static idx_t next_candidate = 1;     /* next integer to examine */

/* public routine requested by the build system */
seq_t a(idx_t n)
{
    /* extend the cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        idx_t x = next_candidate;
        seq_t sum = (seq_t)0;

        /* compute the sum of factorials of the decimal digits of x */
        while (x > 0) {
            int d = (int)(x % 10);
            sum += fact[d];
            x /= 10;
        }

        /* if the sum is a power of two, store the candidate */
        if (is_power_of_two(sum)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }

        next_candidate++;
    }

    /* cache is 0‑based, but a(1) is at cache[0] */
    return cache[(size_t)n - 1];
}
