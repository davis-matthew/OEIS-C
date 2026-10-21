/*
 * A050756  Ninth powers containing no pair of consecutive equal digits.
 * https://oeis.org/A050756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   grow_seq_array, no_consecutive_equal_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A050756: Ninth powers with no pair of consecutive equal digits.
   a(n) is the nth integer m such that m = k^9 for some k (starting at k = 0)
   and the decimal representation of m has no two consecutive equal digits. */
#include <stddef.h>
#include "../utils/no_consecutive_equal_digits.h"
#include "../utils/grow_seq_array.h"

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
