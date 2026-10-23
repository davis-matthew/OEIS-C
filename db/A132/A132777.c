/*
 * A132777  Nonsquare numbers which are the sum of 2 distinct squares.
 * https://oeis.org/A132777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A132777: Nonsquare numbers which are the sum of two distinct squares
 * Computation: a(n) is the n‑th integer (n ≥ 1) that can be expressed as 
 * a² + b² with a, b ∈ ℕ, a ≠ b, and that integer itself is not a perfect square. 
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"

static seq_t *terms      = NULL;   /* dynamically sized array of found terms   */
static size_t cap        = 0;       /* its current capacity                     */
static size_t stored     = 0;       /* number of terms stored so far            */
static idx_t  last_checked = 0;     /* largest integer that has been examined   */

/* --------------------------------------------------------------------- */
/* Helper: true if m can be written as a² + b² with 1 ≤ a < b or 1 ≤ b < a 
 * and m is not a perfect square.                                        */
/* --------------------------------------------------------------------- */
static bool is_sum_of_two_distinct_squares(seq_t m)
{
    /* Smallest such number is 5 (1²+2²). */
    if (m < 5) return false;
    
    /* If m itself is a perfect square, reject.                         */
    seq_t sm = floor_sqrt(m);
    if (sm * sm == m) return false;

    /* Search for a from 1 up to sqrt(m-1).                              */
    seq_t lim = floor_sqrt(m - 1);
    for (seq_t a = 1; a <= lim; ++a) {
        seq_t a2   = a * a;
        seq_t b2   = m - a2;
        if (b2 <= 0) continue;
        seq_t b    = floor_sqrt(b2);
        if (b * b == b2 && b != a) return true;
    }
    return false;
}

/* --------------------------------------------------------------------- */
/* Append a new value to the dynamic array, growing it as necessary.     */
/* --------------------------------------------------------------------- */
static void append_term(seq_t val)
{
    if (stored == cap) {
        size_t new_cap = cap ? cap * 2 : 8;
        seq_t *temp    = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!temp) { abort(); }
        terms = temp;
        cap   = new_cap;
    }
    terms[stored++] = val;
}

/* --------------------------------------------------------------------- */
/* Public function required by the build.                                 */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* behavior for 0 is undefined */

    /* Generate further terms until we have at least n stored.          */
    idx_t i = last_checked + 1;
    while (stored < n) {
        seq_t m = (seq_t)i;
        if (is_sum_of_two_distinct_squares(m)) {
            append_term(m);
        }
        ++i;
    }
    last_checked = i - 1;           /* largest integer examined */

    return terms[n - 1];
}
