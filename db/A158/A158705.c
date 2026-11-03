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
 * Validated: standard, pass, 80 terms through n = 9665 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* parity of bits at even positions of a 64‑bit integer */
static bool odd_even_bits(uint64_t x)
{
    /* mask for even indices: 0101… in binary */
    static const uint64_t mask = 0x5555555555555555ULL;
    uint64_t bits = x & mask;

    /* compute parity by repeatedly toggling a flag for each set bit */
    int parity = 0;
    while (bits) {
        parity ^= 1;
        bits &= bits - 1;          /* clear lowest set bit */
    }
    return parity != 0;              /* true if odd count */
}

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
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) abort();            /* out of memory */
            cache = tmp;
            cache_capacity = new_cap;
        }

        /* store the new value */
        cache[cache_size++] = (seq_t)next_check;
    }

    return cache[n - 1];
}
