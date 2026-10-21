/*
 * A216580  Numbers which can be written in the form x^2 + 10*y^2, where x > 0
 *          and y > 0.
 * https://oeis.org/A216580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A216580: numbers that can be written as x^2 + 10*y^2 (x>0, y>0).
   The function builds the sequence incrementally, caching terms so that
   calls in any order are served quickly. */
/* Ensure we can use realloc. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Static cache for the sequence. */
static seq_t *seq      = NULL;   /* array of terms */
static size_t seq_len  = 0;      /* number of terms stored */
static size_t seq_cap  = 0;      /* current capacity of seq */

/* Grows the cache in place. */
static void grow_seq(void)
{
    size_t new_cap = seq_cap ? seq_cap * 2 : 32;
    seq_t *new_seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (new_seq == NULL)
        return;                 /* allocation failed – future callers will see garbage */
    seq = new_seq;
    seq_cap = new_cap;
}

/* Test whether v can be expressed as x^2 + 10*y^2 with x>0, y>0. */
static bool can_represent(seq_t v)
{
    for (idx_t y = 1;; ++y)
    {
        seq_t y_sq = (seq_t)y * (seq_t)y;   /* y^2 */
        seq_t ten_y_sq = 10 * y_sq;         /* 10*y^2 */
        if (ten_y_sq >= v)
            break;                         /* larger y will never work */
        seq_t rem = v - ten_y_sq;           /* must be a perfect square */
        seq_t x = floor_sqrt(rem);
        if (x > 0 && x * x == rem)
            return true;
    }
    return false;
}

/* Ensure that at least n terms are available in seq. */
static void ensure_terms(idx_t n)
{
    static seq_t last_value = 0;         /* largest term already generated */
    while (seq_len < (size_t)n)
    {
        seq_t candidate = last_value + 1;
        while (!can_represent(candidate))
            ++candidate;

        if (seq_len == seq_cap)
            grow_seq();
        seq[seq_len++] = candidate;
        last_value = candidate;
    }
}

/* External linkage required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0)       /* behavior for index 0 is undefined – protect array access */
        return 0;
    ensure_terms(n);
    return seq[n - 1];
}
