/*
 * A044158  Numbers n such that string 2,6 occurs in the base 7 representation
 *          of n but not of n-1.
 * https://oeis.org/A044158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: Numbers n such that the base‑7 representation of n contains
   the substring "26" but the representation of n-1 does not.  The
   function a(n) returns the nth such number, indexed from 1. */

#include <stdlib.h>
#include <stdbool.h>

/* Does the base‑7 representation of x contain the pair 26? */
static bool has26(seq_t x)
{
    if (x < (seq_t)7)
        return false;        /* cannot contain 2 digits */

    int prev = (int)(x % (seq_t)7);
    x /= (seq_t)7;
    while (x > (seq_t)0) {
        int cur = (int)(x % (seq_t)7);
        if (cur == 2 && prev == 6)
            return true;
        prev = cur;
        x /= (seq_t)7;
    }
    return false;
}

/* Cached sequence values. */
static seq_t *cache      = NULL;   /* pointer to the cache array */
static size_t cache_cap  = 0;      /* allocated capacity */
static size_t cache_cnt  = 0;      /* number of values stored */

/* Next natural number to test.  Stored as seq_t so it works with arbitrary‑precision builds. */
static seq_t next_candidate = (seq_t)1;

static void cache_append(seq_t val)
{
    if (cache_cnt >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_cnt++] = val;
}

/* Return the n‑th number whose base-7 representation contains 26
   but the representation of n‑1 does not.  a(1) is 20. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return 0;   /* undefined, but avoid UB */

    while (cache_cnt < n) {
        if (has26(next_candidate) && !has26(next_candidate - (seq_t)1))
            cache_append(next_candidate);
        ++next_candidate;
    }

    return cache[n - 1];
}
