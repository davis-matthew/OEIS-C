/*
 * A059142  A hierarchical sequence (S(W2{3}*) - see A059126).
 * https://oeis.org/A059142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -25 + 36*2^n - 7*n
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 26, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 2^exp as a seq_t using binary exponentiation */
static seq_t pow2_seq(idx_t exp) {
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)2;
    while (exp) {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

/* OEIS sequence A059142 */
seq_t a(idx_t n) {
    /* a(n) = -25 + 36*2^n - 7*n */
    seq_t pow2n = pow2_seq(n);                 /* 2^n */
    seq_t term  = pow2n * (seq_t)36;           /* 36*2^n */
    seq_t sub1  = (seq_t)(7 * n);              /* 7*n */
    term = term - sub1;                       /* 36*2^n - 7*n */
    term = term - (seq_t)25;                  /* -25 */
    return term;
}
