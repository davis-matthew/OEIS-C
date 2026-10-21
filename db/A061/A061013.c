/*
 * A061013  Numbers k such that (product of digits of k) is divisible by (sum
 *          of digits of k), where 0's are not permitted.
 * https://oeis.org/A061013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 9118, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, get_digits, has_zero_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A061013: Numbers k such that (product of digits of k) is
   divisible by (sum of digits of k). 0‑digits are forbidden.  This
   implementation tests successive integers, skips those containing
   0, and stores found terms in a growing cache so that repeated
   calls are fast.  It works for any seq_t type (small integers or
   arbitrary‑precision). */

#include <stdlib.h>
#include <stdint.h>
/* utility functions that operate on seq_t */
#include "../utils/digit_sum.h"     /* seq_t digit_sum(seq_t); */
#include "../utils/has_zero_digit.h"/* bool has_zero_digit(seq_t); */
#include "../utils/get_digits.h"    /* void get_digits(seq_t, uint8_t *, size_t *); */

static seq_t *terms    = NULL;   /* cached terms */
static size_t terms_cap = 0;     /* capacity of the cache */
static size_t terms_len = 0;     /* number of cached terms */
static seq_t last_checked = 0;   /* last integer examined */

/* enlarge the terms array when needed */
static void grow_terms(void) {
    size_t new_cap = terms_cap == 0 ? 32 : terms_cap * 2;
    seq_t *p = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!p) abort();          /* unrecoverable memory error */
    terms     = p;
    terms_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;     /* undefined for n==0 */

    /* generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        /* check next integer */
        seq_t x = last_checked + 1;
        last_checked = x;

        if (has_zero_digit(x)) continue;   /* skip numbers with 0 */

        seq_t sum_seq = digit_sum(x);
        uint64_t sum = (uint64_t)sum_seq;  /* sum fits in 64 bits */
        if (sum == 0) continue;            /* safety */

        /* compute product of digits modulo sum */
        uint64_t prod_mod = 1 % sum;
        uint8_t buf[256];    /* sufficient for all ordinary seq_t sizes */
        size_t len = 0;
        get_digits(x, buf, &len);

        for (size_t i = 0; i < len; ++i) {
            prod_mod = (prod_mod * buf[i]) % sum;
        }

        if (prod_mod == 0) {         /* k satisfies the condition */
            if (terms_len == terms_cap) grow_terms();
            terms[terms_len++] = x;
        }
    }

    return terms[n - 1];
}
