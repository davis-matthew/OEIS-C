/*
 * A134550  Concatenate Fibonacci(n), Fibonacci(n+2) and Fibonacci(n+4).
 * https://oeis.org/A134550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concat(Fibonacci(n), Fibonacci(n+2), Fibonacci(n+4)) *
 *            where F(0)=0, F(1)=1. * Leading zeros are omitted
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 14, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return number of decimal digits of a value (0 has one digit). */
static inline idx_t digit_len_seq(seq_t v)
{
    if (v == 0) return 1;
    idx_t len = 0;
    while (v > 0) {
        v /= (seq_t)10;
        ++len;
    }
    return len;
}

/* Compute 10^exp as seq_t. */
static inline seq_t pow10_seq(idx_t exp)
{
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i) r *= (seq_t)10;
    return r;
}

/* Compute a(n). */
seq_t a(idx_t n)
{
    /* Compute Fibonacci(n), Fibonacci(n+2), Fibonacci(n+4) by single loop. */
    seq_t fn = 0, fn2 = 0, fn4 = 0;
    seq_t f_prev = 0; /* F(0) */
    seq_t f_cur  = 1; /* F(1) */
    if (n == 0) fn = f_prev;
    if (n == 1) fn = f_cur;
    if ((idx_t)2 == n) fn = f_prev + f_cur; /* will be set below */

    idx_t target1 = n;
    idx_t target2 = n + 2;
    idx_t target3 = n + 4;
    idx_t limit = target3;

    for (idx_t i = 0; i <= limit; ++i) {
        if (i == target1) fn = f_prev;
        if (i == target2) fn2 = f_prev;
        if (i == target3) fn4 = f_prev;
        /* advance Fibonacci */
        seq_t f_next = f_prev + f_cur;
        f_prev = f_cur;
        f_cur = f_next;
    }

    /* Number of digits for the lower two parts. */
    idx_t d2 = digit_len_seq(fn2);
    idx_t d3 = digit_len_seq(fn4);

    seq_t pow1 = pow10_seq(d2 + d3); /* shift for first part */
    seq_t pow2 = pow10_seq(d3);       /* shift for second part */

    return fn * pow1 + fn2 * pow2 + fn4;
}
