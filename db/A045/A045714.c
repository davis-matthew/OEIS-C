/*
 * A045714  Primes with first digit 8.
 * https://oeis.org/A045714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 127, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th prime whose decimal representation
 * starts with the digit 8.  It searches candidate numbers in
 * increasing order, tests primality with is_prime_small, and stores
 * the found primes in a static cache so that future calls (even out of
 * order) are fast.  The algorithm is O(n) for the n‑th term. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cached primes */
    static idx_t sz   = 0;          /* number of cached primes */
    static size_t cap = 0;          /* allocated capacity of cache */
    static uint64_t last_checked = 7; /* start just below the first candidate */

    while (sz < n) {
        ++last_checked;

        /* skip numbers that do not start with the digit 8 */
        uint64_t x = last_checked;
        while (x >= 10) x /= 10;
        if (x != 8) continue;

        /* primality test */
        if (is_prime_small((seq_t)last_checked)) {
            /* grow the cache if necessary */
            if (sz == cap) {
                size_t new_cap = cap == 0 ? 4 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cap = new_cap;
            }
            cache[sz++] = (seq_t)last_checked;
        }
    }
    return cache[n - 1];
}
