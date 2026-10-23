/*
 * A048444  Take the first n numbers written in base 13, concatenate them,
 *          then convert from base 13 to base 10.
 * https://oeis.org/A048444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A048444: concatenate the base‑13 representations of 1..n and interpret the
   resulting concatenation as a decimal (base‑10) integer. */
#include <stddef.h>

static const seq_t POW13[5] = {
    (seq_t)1,    /* 13^0 */
    (seq_t)13,   /* 13^1 */
    (seq_t)169,  /* 13^2 */
    (seq_t)2197, /* 13^3 */
    (seq_t)28561 /* 13^4 */
};

seq_t a(idx_t n)
{
    seq_t value = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t x = k;
        int len = 0;
        while (x) { ++len; x /= 13; }   /* number of base‑13 digits of k */
        value = value * POW13[len] + (seq_t)k;
    }
    return value;
}
