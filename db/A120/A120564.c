/*
 * A120564  Numbers n such that n together with its double and triple contain
 *          every digit.
 * https://oeis.org/A120564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A120564: the n‑th integer m such that the decimal
 * representations of m, 2m, and 3m together contain all digits 0–9.
 * This implementation searches increasing integers and memoises the
 * results. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_counts.h"      /* digit_counts */

static seq_t *cache      = NULL; /* found terms */
static size_t cache_size = 0;     /* number of terms stored */
static size_t cache_cap  = 0;     /* capacity of the array */
static seq_t next_candidate = (seq_t)1; /* next integer to test */

/* Return true iff the digits 0–9 all appear in the decimal
 * representations of m, 2m and 3m. */
static bool term_has_all_digits(seq_t m)
{
    unsigned int mask = 0;
    unsigned char cnt[10];

    /* m */
    digit_counts(m, cnt);
    for (int d = 0; d < 10; ++d)
        if (cnt[d]) mask |= (1u << d);

    /* 2m */
    seq_t two = m + m;
    digit_counts(two, cnt);
    for (int d = 0; d < 10; ++d)
        if (cnt[d]) mask |= (1u << d);

    /* 3m */
    seq_t three = two + m;
    digit_counts(three, cnt);
    for (int d = 0; d < 10; ++d)
        if (cnt[d]) mask |= (1u << d);

    return mask == 0x3FF; /* all ten bits set */
}

/* Grow the cache array as needed. */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort(); /* out of memory */
    cache      = tmp;
    cache_cap  = new_cap;
}

seq_t a(idx_t n)
{
    /* Ensure the cache contains at least n terms. */
    while (cache_size < n) {
        /* Find the next valid term. */
        while (!term_has_all_digits(next_candidate))
            ++next_candidate;

        /* Store the term. */
        if (cache_size == cache_cap)
            grow_cache();
        cache[cache_size++] = next_candidate;

        /* Advance for the next iteration. */
        ++next_candidate;
    }

    /* Indexing starts at 1. */
    return cache[n - 1];
}
