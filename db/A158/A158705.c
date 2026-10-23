/*
 * A158705  Nonnegative integers with an odd number of even powers of 2 in
 *          their base-2 representation.
 * https://oeis.org/A158705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th nonnegative integer whose binary representation
 *            contains an odd number of bits set at even positions (0‑based)
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   odd_even_bits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/odd_even_bits.h"

/* cache of already computed sequence values */
static seq_t *cache = NULL;
static size_t cache_size = 0;      /* number of values stored */
static size_t cache_capacity = 0;  /* allocated capacity */
static uint64_t next_check = 0;    /* next integer to test */

/* one entry of the sequence */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, but safe */

    /* grow cache until it contains at least n values */
    while (cache_size < n) {
        /* find next integer with odd parity of even‑position bits */
        do {
            ++next_check;
        } while (!odd_even_bits(next_check));

        /* resize the cache if needed */
        if (cache_size == cache_capacity) {
            size_t new_cap = cache_capacity ? cache_capacity * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();            /* out of memory */
            cache = tmp;
            cache_capacity = new_cap;
        }

        /* store the new value */
        cache[cache_size++] = (seq_t)next_check;
    }

    return cache[n - 1];
}
