/*
 * A107976  First differences give the same sequence written as a string of
 *          individual digits.
 * https://oeis.org/A107976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   ensure_seq_array, seq_to_dec_string
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A107976
   a(1) = 7
   For n > 1, a(n) = a(n-1) + d(n-1)
   where d(k) is the k‑th digit of the decimal concatenation of a(1), a(2), …, a(k).
   The first differences of the sequence are in fact its own decimal digits. */

#include <stdlib.h>
#include <string.h>
#include "../utils/seq_to_dec_string.h"
#include "../utils/ensure_seq_array.h"

/* External linkage as required */
seq_t a(idx_t n)
{
    /* ---------- Static memoisation state ---------- */
    static seq_t *terms      = NULL;      /* array of terms                */
    static size_t   terms_cap = 0;         /* capacity of terms array       */
    static idx_t    terms_size = 0;        /* number of terms stored (>=1 if initialized) */

    static char   *digits   = NULL;        /* concatenated decimal digits   */
    static size_t   digits_cap = 0;         /* capacity of digits array      */
    static idx_t    digits_len  = 0;         /* current length of digits      */
    static idx_t    digits_pos  = 0;         /* next digit to consume         */

    /* ---------- Initialise on first call ---------- */
    if (terms_size == 0) {
        /* reserve space for the first term */
        ensure_seq_array(&terms, &terms_cap, 8);
        terms[0] = (seq_t)7;
        terms_size = 1;

        /* initial storage for digits */
        digits_cap = 64;
        digits = (char *)malloc(digits_cap);

        digits_len = 0;
        digits_pos = 0;

        /* append digits of the first term */
        char buf[64];
        seq_to_dec_string((seq_t)7, buf, sizeof buf);
        size_t blen = strlen(buf);
        if (digits_len + blen > digits_cap) {
            digits_cap = digits_len + blen + 64;
            digits = (char *)realloc(digits, digits_cap);
        }
        memcpy(digits + digits_len, buf, blen);
        digits_len += blen;
    }

    /* ---------- Grow sequence until we have n terms ---------- */
    while (terms_size < n) {
        /* consume next digit as the difference */
        if (digits_pos >= digits_len) {
            /* This should never happen for generated data */
            return (seq_t)0;
        }
        int d = digits[digits_pos++] - '0';

        /* compute the new term */
        seq_t new_val = terms[terms_size - 1] + (seq_t)d;

        /* ensure capacity for the terms array */
        if ((size_t)terms_size >= terms_cap) {
            ensure_seq_array(&terms, &terms_cap, terms_size + 8);
        }
        terms[terms_size++] = new_val;

        /* append decimal digits of the new term */
        char buf[128];
        seq_to_dec_string(new_val, buf, sizeof buf);
        size_t blen = strlen(buf);
        if (digits_len + blen > digits_cap) {
            while (digits_len + blen > digits_cap)
                digits_cap *= 2;
            digits = (char *)realloc(digits, digits_cap);
        }
        memcpy(digits + digits_len, buf, blen);
        digits_len += blen;
    }

    return terms[n - 1];
}
