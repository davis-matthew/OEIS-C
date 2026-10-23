/*
 * A039382  Numbers whose base-9 representation has the same nonzero number of
 *          3's and 8's.
 * https://oeis.org/A039382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers whose base‑9 representation has the same positive number
 *            of 3's and 8's
 * Range:     u8 n <= 7, u16 n <= 7663, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_equal_3_8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/has_equal_3_8.h"

/* Static storage for the generated sequence */
static seq_t  *terms      = NULL;   /* array of found terms */
static size_t  terms_cap  = 0;      /* current capacity of the array */
static size_t  terms_cnt  = 0;      /* number of terms stored */
static uint64_t next_int   = 1;     /* next integer to test */

/* Grow the terms array when needed */
static void grow_terms(void)
{
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);          /* abort on allocation failure */
    terms = tmp;
    terms_cap = new_cap;
}

/* Ensure that at least n terms have been generated */
static void ensure_terms(idx_t n)
{
    while (terms_cnt < n) {
        /* Find the next integer satisfying the condition */
        while (!has_equal_3_8(next_int))
            ++next_int;

        /* Store the term */
        if (terms_cnt >= terms_cap) grow_terms();
        terms[terms_cnt++] = (seq_t)next_int;
        ++next_int; /* prepare for the next search */
    }
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; behaviour for n == 0 is
       irrelevant to the tests. */
    if (n == 0) return 0;
    ensure_terms(n);
    return terms[n-1];
}
