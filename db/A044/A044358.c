/*
 * A044358  Numbers n such that string 2,6 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 712, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   seq_realloc, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A044358: a(n) = nth integer whose decimal representation contains
   substring "26" while (n-1) does not.  This implementation
   computes the terms on demand and memoises them so that calls may
   arrive in any order. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_to_str.h"
#include "../utils/seq_realloc.h"

/* Static cache of discovered terms */
static seq_t *cache = NULL;   /* array of sequence values   */
static size_t count = 0;      /* number of terms stored    */
static size_t capacity = 0;   /* allocated capacity        */

/* Test whether decimal string of x contains "26" */
static bool has26(seq_t x)
{
    /* A buffer big enough for the decimal representation of any
       seq_t value that appears for the requested n values.  The
       sequence grows much slower than the range of seq_t so 4096
       bytes is ample. */
    static char buf[4096];
    size_t len;

    seq_to_str(x, buf, &len);

    for (size_t i = 0; i + 1 < len; ++i)
        if (buf[i] == '2' && buf[i + 1] == '6')
            return true;

    return false;
}

/* Ensure that the cache contains at least 'target' terms. */
static void extend_cache(size_t target)
{
    if (target <= count)
        return;

    /* Grow the array if necessary. */
    if (capacity < target) {
        size_t new_cap = capacity ? capacity * 2 : 16;
        if (new_cap < target)
            new_cap = target;
        seq_t *tmp = seq_realloc(cache, capacity, new_cap);
        cache = tmp;
        capacity = new_cap;
    }

    /* Scan forward from the next integer after the last cached value. */
    static seq_t last_checked = 0;
    seq_t cand = last_checked + 1;

    while (count < target) {
        bool cand_has = has26(cand);
        bool prev_has = has26(cand - 1);
        if (cand_has && !prev_has)
            cache[count++] = cand;
        cand = cand + 1;
    }

    last_checked = cand - 1;
}

/* Public interface: returns the nth term of A044358. */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n < 1, but safe return */
        return (seq_t)0;

    extend_cache((size_t)n);
    return cache[(size_t)n - 1];
}
