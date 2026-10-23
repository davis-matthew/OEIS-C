/*
 * A044189  Numbers n such that string 0,2 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 1142, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function generates OEIS A044189: the n‑th number whose base‑8
   representation contains the substring "02" while the preceding number
   does not.  The implementation simply scans integers in order, testing
   the property, and memoises results so that repeated calls to a() are
   fast. */

#include <stdlib.h>
#include <stdbool.h>

/* Return true iff the octal representation of x contains the
   adjacent digits 0 then 2. */
static bool has_02(seq_t x)
{
    if (x < 2u) return false;   /* less than two digits can never contain "02" */
    seq_t prev = x % (seq_t)8;
    x      /= (seq_t)8;
    while (x > 0u) {
        seq_t cur = x % (seq_t)8;
        if (cur == 0u && prev == 2u) return true;
        prev = cur;
        x   /= (seq_t)8;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0u) return 0;   /* safety; sequence is 1‑based */

    static seq_t *cache = NULL;      /* memoised sequence values */
    static size_t cache_len = 0;     /* number of values stored */
    static size_t cache_cap = 0;     /* capacity of the allocated array */
    static seq_t last_value = 0;     /* last value stored in cache */

    /* Ensure the cache holds at least n terms */
    while (cache_len < (size_t)n) {
        seq_t cand = last_value + (seq_t)1;

        /* Find the next number satisfying the definition */
        while (true) {
            if (has_02(cand) && !has_02(cand - (seq_t)1))
                break;
            ++cand;
        }

        /* Expand the cache if necessary */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0u) ? 16u : cache_cap * 2u;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();        /* out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = cand;
        last_value = cand;
    }

    return cache[n - 1];
}
