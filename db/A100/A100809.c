/*
 * A100809  Numbers that are a sum of two squares and not divisible by 13.
 * https://oeis.org/A100809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   integer_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers that are sums of two *positive* squares and not divisible by 13.
   The function searches integers in increasing order, checks the two
   conditions, and caches the found terms for fast repeated access. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/integer_sqrt.h"

/* Test whether an integer n can be written as a^2 + b^2 with a>0 and b>0. */
static bool has_positive_sum_of_two_squares(idx_t n)
{
    if (n < 2)                /* 0,1 cannot be represented with two positive squares */
        return false;
    idx_t max_a = integer_sqrt(n - 1);        /* a^2 must be < n, so a <= sqrt(n-1) */
    for (idx_t a = 1; a <= max_a; ++a)
    {
        idx_t a_sq = a * a;                   /* a^2 fits in idx_t (n <= 2^64-1) */
        idx_t b_sq = n - a_sq;
        idx_t b = integer_sqrt(b_sq);
        if (b * b == b_sq && b > 0)           /* found a positive b */
            return true;
    }
    return false;
}

/* Static storage for computed terms. */
static seq_t *terms          = NULL;
static idx_t  capacity       = 0;
static idx_t  count          = 0;
static idx_t  next_candidate = 2;

/* Expand the terms array as needed. */
static void grow_terms(idx_t required)
{
    if (capacity >= required)
        return;
    size_t new_cap = capacity ? capacity << 1 : 4;
    while (new_cap < required)
        new_cap <<= 1;
    seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_terms)
        abort();                      /* out of memory */
    terms = new_terms;
    capacity = new_cap;
}

/* Main function requested by the OEIS interface. */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset at 1 – behaviour for n<1 does not matter */
        return 0;

    /* Ensure the table contains at least n terms. */
    while (count < n)
    {
        /* Find the next integer satisfying both conditions. */
        while ((next_candidate % 13ULL) == 0 || !has_positive_sum_of_two_squares(next_candidate))
            ++next_candidate;

        grow_terms(count + 1);
        terms[count++] = (seq_t)next_candidate;
        ++next_candidate;
    }
    return terms[n - 1];
}
