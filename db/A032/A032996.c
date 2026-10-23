/*
 * A032996  If d,e are consecutive digits of n in base 8, then |d-e|>=5.
 * https://oeis.org/A032996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   good_octal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A032996 – numbers n such that every pair of consecutive
 * octal digits differ by at least 5 in absolute value.  The function a(n)
 * returns the nth term (offset 1).  The implementation maintains a
 * simple cache of successive valid numbers; the next candidate to
 * test is stored in a static variable so that repeated calls with
 * arbitrary n do not re‑start from the beginning.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/good_octal.h"

/* Static cache of successful numbers.  The first index [0] holds the
 * first element of the sequence (i.e. a(1)). */
static seq_t *cache       = NULL;
static idx_t  size        = 0;   /* number of elements stored   */
static idx_t  capacity    = 0;   /* allocated elements          */

/* Next candidate to test after the current cache has been built.  It
 * starts at 1 (the first non‑zero positive integer). */
static seq_t next_candidate = (seq_t)1;

/* Grow the cache buffer to hold at least `need` elements. */
static void grow_cache(idx_t need)
{
    if (need <= capacity)
        return;
    idx_t new_cap = (capacity == 0) ? 16 : capacity * 2;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *new_arr = (seq_t *)realloc(cache,
                                      (size_t)new_cap * sizeof(seq_t));
    if (!new_arr)
        abort();
    cache = new_arr;
    capacity = new_cap;
}

/* Ensure that the cache contains the first `target` terms of the sequence. */
static void generate_up_to(idx_t target)
{
    if (size >= target)
        return;

    seq_t candidate = next_candidate;

    /* The loop stops once we have found `target` valid numbers. */
    while (size < target) {
        if (good_octal(candidate)) {
            grow_cache(size + 1);
            cache[size++] = candidate;
        }
        candidate++;
    }

    /* Record the next number that should be examined in future calls. */
    next_candidate = candidate;
}

seq_t a(idx_t n)
{
    if (n == 0)              /* undefined for the OEIS offset, but keep 0 */
        return (seq_t)0;
    generate_up_to(n);
    return cache[n - 1];
}
