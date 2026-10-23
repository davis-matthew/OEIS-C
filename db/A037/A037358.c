/*
 * A037358  Numbers whose base-5 and base-6 expansions have no digits in
 *          common.
 * https://oeis.org/A037358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 621, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence: Numbers whose base‑5 and base‑6 expansions have no digits in common.
 *
 *  To generate the n-th term (a(1)=5) we scan natural numbers building all values
 *  whose digit sets in base 5 and base 6 are disjoint.  The sequence is stored
 *  in a dynamically expanding cache so that calls in arbitrary order are
 *  answered efficiently.  Each value is tested by extracting its digits in
 *  base 5 and base 6 and forming two bit masks.  If the masks have no common
 *  bit the number is kept in the cache; otherwise it is rejected.  The
 *  algorithm runs in O(n) time with very small constants, easily fitting
 *  the 2.0 s requirement for n up to about 10 000.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Test whether a number v has no common digits in its base‑5 and base‑6 representations. */
static bool has_no_common_digits(seq_t v)
{
    unsigned int mask5 = 0, mask6 = 0;
    seq_t t = v;

    /* If v == 0, it has digit 0 in both bases, so reject (not needed for this sequence). */
    if (t == 0) return false;

    /* Mask for base‑5 digits */
    while (t != 0) {
        unsigned int d5 = (unsigned int)(t % 5);
        mask5 |= 1U << d5;
        t /= 5;
    }

    t = v;
    /* Mask for base‑6 digits */
    while (t != 0) {
        unsigned int d6 = (unsigned int)(t % 6);
        mask6 |= 1U << d6;
        t /= 6;
    }

    return (mask5 & mask6) == 0;
}

/* --------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Static state: cache, its capacity, the number of cached terms,
       and the next value to test. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    static seq_t next_candidate = 0;

    /* Grow the cache until it holds at least n terms. */
    while (cache_size < n) {
        /* Find the next valid number. */
        do {
            next_candidate++;
        } while (!has_no_common_digits(next_candidate));

        /* Resize the cache if necessary. */
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!new_cache) return 0;  /* OOM – unlikely in normal usage. */
            if (cache) {
                for (size_t i = 0; i < cache_size; ++i) {
                    new_cache[i] = cache[i];
                }
                free(cache);
            }
            cache = new_cache;
            cache_cap = new_cap;
        }

        cache[cache_size++] = next_candidate;
    }

    return cache[n - 1];
}
