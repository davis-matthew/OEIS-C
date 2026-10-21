/*
 * A083956  a(n) = sum of all cyclic permutation of concatenation of first n
 *          numbers. In each case the digits of a number are kept together for
 *          n>9.
 * https://oeis.org/A083956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = Σ_{i=0}^{n-1} ( suffix_i * 10^{digits(1..i)} + prefix_i )
 *            where suffix_i = concat of numbers i+1..n, and prefix_i = concat
 *            of 1..i. This summation produces OEIS A083956
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   digit_count, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/digit_count.h"   /* uint64_t digit_count(uint64_t n) */
#include "../utils/pow10.h"         /* seq_t pow10(idx_t e) */

/* compute the n-th term of OEIS A083956 */
seq_t a(idx_t n)
{
    idx_t i;

    /* First pass: build the full concatenation S_n, its digit length,
       and the initial suffix (which is S_n itself). */
    seq_t suffix = 0;          /* suffix_0 = concat(1..n) */
    idx_t total_len = 0;       /* total number of digits in S_n */

    for (i = 1; i <= n; ++i) {
        idx_t d = digit_count(i);              /* digits of the number i */
        total_len += d;
        seq_t powd = pow10(d);                 /* 10^d */
        suffix = suffix * powd + (seq_t)i;      /* append i to the right */
    }

    /* Variables maintained over the rotations */
    seq_t sum = 0;                /* accumulation of all terms */
    seq_t pref = 0;               /* prefix_i = concat(1..i) */
    seq_t pow_pref = 1;           /* 10^{digits(1..i)} ; initially 1 */
    idx_t digits_pref = 0;        /* digits(1..i) */
    idx_t digits_suffix = total_len;  /* digits(i+1..n) currently */

    for (i = 0; i < n; ++i) {
        /* Current term: suffix_i * 10^{digits_pref} + prefix_i */
        seq_t term = suffix * pow_pref + pref;
        sum += term;

        /* Stop after the last rotation is processed */
        if (i + 1 == n) {
            break;
        }

        /* Prepare data for the next rotation (i -> i+1) */

        /* Next number to append to the prefix */
        idx_t next_digit = digit_count(i + 1);
        seq_t pow_next = pow10(next_digit);              /* 10^{digits(next)} */

        /* Update prefix and its power of ten */
        pref = pref * pow_next + (seq_t)(i + 1);
        pow_pref = pow_pref * pow_next;
        digits_pref += next_digit;

        /* Update suffix: remove the leading number (i+1) from it */
        digits_suffix -= next_digit;                     /* new suffix length */
        seq_t pow_suf_new = pow10(digits_suffix);        /* 10^{digits_suffix} */
        seq_t subfactor = (seq_t)(i + 1) * pow_suf_new;   /* value to subtract */
        suffix = suffix - subfactor;
    }

    return sum;
}
