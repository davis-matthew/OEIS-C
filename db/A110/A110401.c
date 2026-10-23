/*
 * A110401  a(1) = 999, a(n) is the number obtained by concatenating product
 *          of neighboring digits of the previous term.
 * https://oeis.org/A110401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 7, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   pow10, seq_malloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 999; 
   a(n) = concatenate the products of adjacent decimal digits of a(n‑1). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow10.h"    /* pow10(idx_t exp) -> seq_t */
#include "../utils/seq_malloc.h"   /* seq_malloc(idx_t) -> seq_t * */
#include "../utils/seq_realloc.h"  /* seq_realloc(seq_t *, idx_t) -> seq_t * */

/* --------------------------------------------------------------------- */
/* Helper: compute the next term from the previous term.                 */
/* --------------------------------------------------------------------- */
static seq_t next_term(const seq_t *prev)
{
    seq_t result = 0;

    /* Determine the length of *prev */
    idx_t len = 0;
    seq_t tmp = *prev;
    do {
        ++len;
        tmp /= (seq_t)10;
    } while (tmp != (seq_t)0);

    /* Extract digits (least significant first) */
    int *digits = (int *)malloc(len * sizeof(int));
    tmp = *prev;
    for (idx_t i = 0; i < len; ++i) {
        digits[i] = (int)(tmp % (seq_t)10);
        tmp /= (seq_t)10;
    }

    /* Concatenate products of adjacent digits, from most significant pair */
    for (idx_t i = len - 1; i > 0; --i) {
        int d1 = digits[i];
        int d2 = digits[i - 1];
        int prod = d1 * d2;                      /* 0 <= prod <= 81 */
        seq_t prod_seq = (seq_t)prod;
        idx_t prod_len = (prod >= 10 ? 2 : 1);
        result = result * pow10(prod_len) + prod_seq;
    }

    free(digits);
    return result;
}

/* --------------------------------------------------------------------- */
/* Memoization storage                                                */
/* --------------------------------------------------------------------- */
static seq_t *terms       = NULL;   /* array of computed terms */
static idx_t  terms_size  = 0;      /* number of terms actually stored */
static idx_t  terms_cap   = 0;      /* allocated capacity */

/* Ensure the terms array can hold at least 'need' elements. */
static void ensure_capacity(idx_t need)
{
    if (need <= terms_cap) return;
    idx_t new_cap = (terms_cap == 0) ? 1 : terms_cap;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (terms == NULL)
                 ? seq_malloc(new_cap)
                 : seq_realloc(terms, new_cap);
    terms = tmp;
    terms_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Public interface                                                  */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* unused, but safe */

    if (terms_size < n) {
        ensure_capacity(n);
        for (idx_t i = terms_size; i < n; ++i) {
            if (i == 0) {
                terms[0] = (seq_t)999;        /* a(1) */
            } else {
                terms[i] = next_term(&terms[i - 1]); /* a(i+1) */
            }
        }
        terms_size = n;
    }
    return terms[n - 1];
}
