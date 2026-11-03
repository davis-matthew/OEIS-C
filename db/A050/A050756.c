/*
 * A050756  Ninth powers containing no pair of consecutive equal digits.
 * https://oeis.org/A050756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A050756: Ninth powers with no pair of consecutive equal digits.
   a(n) is the nth integer m such that m = k^9 for some k (starting at k = 0)
   and the decimal representation of m has no two consecutive equal digits. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Test whether a seq_t value has any pair of consecutive equal decimal digits. */
static inline bool no_consecutive_equal_digits(seq_t val) {
    if (val == (seq_t)0) return true;          /* 0 contains only one digit */
    int prev = -1;                             /* previous digit, -1 means none yet */
    seq_t tmp = val;
    while (tmp > (seq_t)0) {
        int d = (int)(tmp % (seq_t)10);        /* current least‑significant digit */
        if (prev == d) return false;           /* found equal neighbours */
        prev = d;
        tmp /= (seq_t)10;                      /* shift right one decimal digit */
    }
    return true;                               /* all adjacent digits differ */
}

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

/* Static state to cache computed terms and to continue computation
   when a later index is requested. */
static seq_t *terms = NULL;   /* array of computed terms */
static size_t terms_len = 0;  /* current allocated length of the array */
static size_t terms_cnt = 0;  /* number of valid terms stored */
static idx_t next_k = 0;      /* next k to test (k^9) */

/* Ensure that the array contains at least (n+1) valid terms. */
static void ensure_terms(idx_t n)
{
    seq_t value;
    int i;
    while (terms_cnt <= (size_t)n) {
        /* compute k^9 */
        value = 1;
        for (i = 0; i < 9; ++i)
            value *= (seq_t)next_k;

        /* keep the term if it satisfies the condition */
        if (no_consecutive_equal_digits(value)) {
            /* grow the array in advance */
            size_t new_len = terms_cnt + 1;
            grow_seq_array(&terms, &terms_len, new_len);
            terms[terms_cnt] = value;
            terms_cnt++;
        }
        next_k++;
    }
}

seq_t a(idx_t n)
{
    ensure_terms(n);
    return terms[n];
}
