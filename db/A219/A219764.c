/*
 * A219764  Numbers whose binary expansion has curling number 1.
 * https://oeis.org/A219764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th nonnegative integer whose binary representation *
 *            has curling number 1. * * The implementation scans successive
 *            integers, tests whether the binary * string ends with any
 *            repeated block, and caches the numbers that * satisfy the
 *            condition. This works for any seq_t (8/16/32/64/128-bit *...
 * Range:     u8 n <= 81, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

/* Return true if the binary expansion of x has curling number 1,
 * i.e. its last l bits are never equal to the preceding l bits for any
 * l ≥ 1.  x == 0 is considered to have curling number 1. */
static bool is_curling_one(seq_t x)
{
    if (x == (seq_t)0)
        return true;                 /* "0" has length 1 */

    unsigned len = bitlen_seq_t(x);   /* number of bits */
    if (len <= 1)
        return true;                  /* single‑bit strings cannot repeat */

    /* Check all possible block lengths l */
    for (unsigned l = 1; l <= len / 2; ++l) {
        seq_t mask = ((seq_t)1 << l) - (seq_t)1;
        seq_t last  = x & mask;
        seq_t prev  = (x >> l) & mask;
        if (last == prev)
            return false;             /* repeated block found */
    }
    return true;                      /* no repetition at the end */
}

seq_t a(idx_t n)
{
    static idx_t    next_candidate = 0;   /* next integer to test   */
    static size_t   count          = 0;   /* how many valid numbers |
                                          * stored                */
    static size_t   capacity       = 0;   /* array capacity         */
    static seq_t  *cache          = NULL; /* array of valid numbers */

    /* Allocate initial capacity if needed */
    if (capacity == 0) {
        capacity = 64;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache)
            abort();
    }

    /* Gather numbers until we have at least n of them */
    while (count < n) {
        seq_t candidate = (seq_t)next_candidate++;   /* test this number */

        if (is_curling_one(candidate)) {
            /* Grow cache if full */
            if (count == capacity) {
                size_t new_cap = capacity * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();
                cache = tmp;
                capacity = new_cap;
            }
            cache[count++] = candidate;
        }
    }

    return cache[n - 1];
}
