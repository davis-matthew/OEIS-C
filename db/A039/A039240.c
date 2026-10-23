/*
 * A039240  Numbers whose base-12 representation has the same number of 3's
 *          and 5's.
 * https://oeis.org/A039240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   There is no closed formula – the sequence is defined as the
 *            increasing list of non‑negative integers whose base‑12
 *            representation contains an equal number of digits 3 and 5
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function: a(n) returns the nth term (offset 1). */

#include <stddef.h>     /* size_t */
#include <stdlib.h>     /* malloc, free */
#include <string.h>     /* memcpy */
#include <stdbool.h>    /* bool */

static seq_t *cache = NULL;      /* stored sequence values */
static size_t cap   = 0;         /* capacity of cache */
static size_t sz    = 0;         /* number of cached values */
static seq_t next   = 0;         /* next candidate to test */

/* test whether x has the same number of 3's and 5's in base‑12 */
static bool good(seq_t x)
{
    int c3 = 0, c5 = 0;
    seq_t y = x;
    while (y > 0) {
        int d = (int)(y % 12);
        if (d == 3)
            ++c3;
        else if (d == 5)
            ++c5;
        y /= 12;
    }
    return c3 == c5;
}

/* ensure the cache has at least new_cap elements allocated */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cap)
        return;
    size_t nc = cap ? cap * 2 : 64;
    while (nc < new_cap)
        nc *= 2;
    seq_t *tmp = (seq_t *)malloc(nc * sizeof(seq_t));
    if (cache) {
        memcpy(tmp, cache, sz * sizeof(seq_t));
        free(cache);
    }
    cache = tmp;
    cap   = nc;
}

seq_t a(idx_t n)
{
    /* n is 1‑based */
    while (sz < n) {
        /* find the next number that satisfies the property */
        while (!good(next))
            ++next;
        /* store it */
        grow_cache(sz + 1);
        cache[sz++] = next;
        ++next;
    }
    return cache[n - 1];
}
