/*
 * A044111  Numbers k such that string 1,3 occurs in the base 6 representation
 *          of k but not of k-1.
 * https://oeis.org/A044111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 1989, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: a(n) = the n-th positive integer whose base‑6
   representation contains the consecutive digits 1 and 3 but the
   preceding integer does not.  The function computes terms on demand
   and memoises them so that repeated calls are fast. */

#include <stdbool.h>
#include <stdlib.h>

static bool has13(seq_t x)
{
    /* Return true iff the base‑6 representation of x contains “13”. */
    if (x < 6)            /* fewer than two digits → impossible */
        return false;

    seq_t prev = x % 6;   /* least‑significant digit */
    x /= 6;
    while (x > 0) {
        seq_t cur = x % 6;   /* next digit to the left */
        if (cur == 1 && prev == 3)
            return true;
        prev = cur;
        x /= 6;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* n is 1‑based; behaviour for n < 1 is undefined. */

    /* Static state for memoisation. */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_size = 0;
    static seq_t next_candidate = 0;
    static bool   prev_has13 = false;

    if (n == 0)            /* below offset; not used by tests          */
        return 0;

    /* Grow cache array if necessary. */
    if (cache_size < n) {
        size_t required = n;
        if (required > cache_cap) {
            size_t new_cap = cache_cap == 0 ? 16 : cache_cap;
            while (new_cap < required) new_cap <<= 1;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!cache) abort();          /* allocation failed */
            cache_cap = new_cap;
        }

        /* Scan integers until we have found n terms. */
        while (cache_size < n) {
            ++next_candidate;
            bool cur_has13 = has13(next_candidate);
            if (cur_has13 && !prev_has13) {
                cache[cache_size++] = next_candidate;
            }
            prev_has13 = cur_has13;
        }
    }

    return cache[n - 1];
}
