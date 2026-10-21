/*
 * A099211  a(n) = -2*a(n-1) + 4*a(n-3), with a(0) = 1, a(1) = -2, a(2) = 4.
 * https://oeis.org/A099211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -2*a(n-1) + 4*a(n-3); a(0)=1, a(1)=-2, a(2)=4
 * Range:     i8 n <= 9, i16 n <= 16, i32 n <= 33, i64 n <= 69, i128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Local memoisation table for previously computed values */
static seq_t *terms       = NULL;       /* array of terms, indexed by n */
static size_t terms_cap   = 0;          /* allocated array capacity */
static size_t last_computed = 0;       /* highest index whose value is valid */

/* Ensure the table can hold indices up to 'need' (inclusive) */
static void ensure_capacity(size_t need)
{
    if (terms_cap > need) return;
    size_t new_cap = terms_cap ? terms_cap : 8;
    while (new_cap <= need) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) abort();                    /* allocation failure */
    terms = tmp;
    terms_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Handle the three base cases directly (no need to touch the array) */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-2;
    if (n == 2) return (seq_t)4;

    /* First call: allocate the table and initialise the base values */
    if (!terms) {
        size_t init_cap = 8;
        while (init_cap < (size_t)n + 1) init_cap <<= 1;
        terms = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!terms) abort();
        terms[0] = (seq_t)1;
        terms[1] = (seq_t)-2;
        terms[2] = (seq_t)4;
        terms_cap = init_cap;
        last_computed = 2;
    }

    /* Ensure we have enough room for n */
    ensure_capacity((size_t)n);

    /* If we already computed up to n, simply return it */
    if (n <= (idx_t)last_computed) return terms[(size_t)n];

    /* Compute all missing terms up to n, using the recurrence */
    for (size_t i = last_computed + 1; i <= (size_t)n; ++i) {
        terms[i] = (seq_t)(-2) * terms[i-1] + (seq_t)4 * terms[i-3];
    }
    last_computed = (size_t)n;
    return terms[(size_t)n];
}
