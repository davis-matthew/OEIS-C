/*
 * A160246  Numerator of Hermite(n, 7/29).
 * https://oeis.org/A160246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 29^n * Hermite(n, 7/29)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = 14, a(n+1) = 14*a(n) - 2*n*29^2*a(n-1) */
/* 29^2 = 841 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)14;

    const seq_t C14    = (seq_t)14;
    const seq_t C841   = (seq_t)841;          /* 29^2 */
    const seq_t C2     = (seq_t)2;

    seq_t prevprev = (seq_t)1;   /* a(0) */
    seq_t prev     = (seq_t)14;  /* a(1) */
    seq_t cur;

    /* Compute a(n) iteratively */
    for (idx_t k = 1; k < n; ++k) {
        /* factor = 2*k*29^2 */
        seq_t factor = C2 * ((seq_t)k) * C841;
        cur = C14 * prev - factor * prevprev;
        prevprev = prev;
        prev = cur;
    }
    return prev;
}
