/*
 * A177361  a(n) contains the nonzero frequencies f(d) of digits d=0 .. 9 in
 *          all terms up to a(n-1) in concatenated form sorted with respect to
 *          d: f(0)//0//f(1)//1//...//f(9)//9. Initial term a(1)=2.
 * https://oeis.org/A177361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 46,
 *            big any n
 * Helpers:   alloc_seq_array, digit_counts, digit_len, free_seq_array, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the concatenation of the non‑zero cumulative digit frequencies
   of 0…9 up to a(n‑1):  f(0)//0//f(1)//1//…//f(9)//9.  a(1)=2. */

#include <stdlib.h>
#include <stdint.h>

/* utilities for arbitrary‑precision integer and digit handling */
#include "../utils/alloc_seq_array.h"   /* seq_t *alloc_seq_array(size_t n); */
#include "../utils/free_seq_array.h"    /* void free_seq_array(seq_t *p); */
#include "../utils/digit_counts.h"     /* void digit_counts(seq_t x, unsigned char cnt[10]); */
#include "../utils/digit_len.h"        /* int digit_len(seq_t x); */
#include "../utils/pow10.h"            /* seq_t pow10(idx_t e); */

/* cached sequence values */
static seq_t *terms   = NULL;   /* array of terms */
static size_t terms_cap = 0;    /* allocated capacity (in terms) */
static size_t terms_len = 0;    /* number of terms stored */

/* cumulative digit frequencies up to the last computed term */
static uint64_t freq[10] = {0};

/* ensure the terms array can hold at least n elements */
static void ensure_terms_capacity(size_t n)
{
    if (terms_cap >= n)
        return;
    size_t new_cap = terms_cap ? terms_cap << 1 : 8;
    while (new_cap < n)
        new_cap <<= 1;
    seq_t *new_terms = alloc_seq_array(new_cap);
    for (size_t i = 0; i < terms_len; ++i)
        new_terms[i] = terms[i];
    free_seq_array(terms);
    terms      = new_terms;
    terms_cap  = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* n is 1‑based; no check needed because callers use n ≥ 1 */
    ensure_terms_capacity((size_t)n);

    /* initialise first term if the cache is empty */
    if (terms_len == 0) {
        terms[0] = (seq_t)2;          /* a(1) = 2 */
        freq[2]  = 1;                 /* digit '2' appears once so far */
        terms_len = 1;
    }

    /* compute any missing terms up to n */
    for (size_t i = terms_len; i < (size_t)n; ++i) {
        seq_t term = (seq_t)0;

        /* build the term from current cumulative frequencies */
        for (unsigned d = 0; d < 10; ++d) {
            if (freq[d] == 0)
                continue;
            seq_t f   = (seq_t)freq[d];
            int   flen = digit_len(f);
            seq_t pow  = pow10((idx_t)flen);
            term = term * pow  + f;   /* append frequency */
            term = term * 10      + d;/* append digit itself   */
        }

        terms[i] = term;

        /* update frequencies with digits of the new term */
        unsigned char cnt[10] = {0};
        digit_counts(term, cnt);
        for (unsigned d = 0; d < 10; ++d)
            freq[d] += cnt[d];
    }

    terms_len = (size_t)n;
    return terms[n - 1];
}
