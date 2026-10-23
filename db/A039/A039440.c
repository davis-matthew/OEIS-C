/*
 * A039440  Numbers whose base-11 representation has the same nonzero number
 *          of 0's and 3's.
 * https://oeis.org/A039440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: A039440 – numbers whose base‑11 representation has the same
   non‑zero number of 0's and 3's.  The function returns the n‑th term
   (offset 1). */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Test whether x satisfies the property in base 11.  The function
   returns true if the numbers of 0's and 3's in the base‑11 expansion
   are equal and non‑zero. */
static bool good(seq_t x)
{
    seq_t y = x;
    int count0 = 0, count3 = 0;
    while (y != 0) {
        seq_t d = y % (seq_t)11;
        if (d == 0) {
            count0++;
        } else if (d == 3) {
            count3++;
        }
        y = y / (seq_t)11;
    }
    return (count0 == count3 && count0 > 0);
}

/* Cache of terms found so far.  These are static so that calls may
   come in any order and still be efficient. */
static seq_t *cache      = NULL;   /* array of terms */
static size_t cache_len  = 0;       /* number of terms stored */
static size_t cache_cap  = 0;       /* allocated capacity */
static seq_t last_value  = 0;       /* last term added */

/* Grow the cache to hold at least 'need' elements. */
static void grow_cache(size_t need)
{
    if (need <= cache_cap)
        return;

    size_t new_cap = (cache_cap > 0) ? cache_cap : 16;
    while (new_cap < need)
        new_cap <<= 1;

    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr)
        exit(1);   /* out of memory */

    /* Copy existing terms */
    for (size_t i = 0; i < cache_len; ++i)
        new_arr[i] = cache[i];

    free(cache);
    cache      = new_arr;
    cache_cap  = new_cap;
}

/* Return the nth term (n >= 1).  For n == 0 this returns 0 as a
   harmless placeholder. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Convert n to a size_t safely for indexing.  n is guaranteed
       to be small enough for size_t in the required tests. */
    size_t target = (size_t)n;

    /* If the cache already contains the required term, return it. */
    if (cache_len >= target)
        return cache[target - 1];

    /* Ensure the cache can hold up to 'target' terms. */
    grow_cache(target);

    /* Start searching from the next candidate after the last one
       we already examined. */
    seq_t cur = (cache_len > 0) ? last_value + (seq_t)1 : (seq_t)1;
    size_t found = cache_len;

    while (found < target) {
        if (good(cur)) {
            cache[found] = cur;
            last_value = cur;
            ++found;
        }
        ++cur;
    }

    cache_len = target;
    return cache[target - 1];
}
