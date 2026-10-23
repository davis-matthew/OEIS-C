/*
 * A163202  Alternating sum of the cubes of the first n odd-indexed Fibonacci
 *          numbers.
 * https://oeis.org/A163202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) + 21*a(n-1) + 56*a(n-2) + 21*a(n-3) + a(n-4) = -28 Initial
 *            values: a(0)=0, a(1)=-1, a(2)=7, a(3)=-118
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 8, i64 n <= 16, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;
static idx_t a_cache_size = 0;

/* Grow the cache to hold values up to 'upto' (inclusive) */
static void a_grow_cache(idx_t upto)
{
    if (upto < a_cache_size)
        return;  /* already has what we need */

    size_t new_size = (size_t)(upto + 1);
    seq_t *new_cache = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
    if (!new_cache && new_size > 0) {
        /* Allocation failed – abort exercise */
        exit(1);
    }
    a_cache = new_cache;

    for (idx_t i = a_cache_size; i <= upto; ++i) {
        size_t idx = (size_t)i;
        if (i == 0)
            a_cache[idx] = (seq_t)0;
        else if (i == 1)
            a_cache[idx] = (seq_t)-1;
        else if (i == 2)
            a_cache[idx] = (seq_t)7;
        else if (i == 3)
            a_cache[idx] = (seq_t)-118;
        else {
            /* a(i) = -28 - 21*a(i-1) - 56*a(i-2) - 21*a(i-3) - a(i-4) */
            seq_t val = (seq_t)-28;
            val -= (seq_t)21 * a_cache[idx-1];
            val -= (seq_t)56 * a_cache[idx-2];
            val -= (seq_t)21 * a_cache[idx-3];
            val -= a_cache[idx-4];
            a_cache[idx] = val;
        }
    }
    a_cache_size = upto + 1;
}

seq_t a(idx_t n)
{
    a_grow_cache(n);
    return a_cache[(size_t)n];
}
