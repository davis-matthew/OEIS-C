/*
 * A069921  Define C(n) by the recursion C(0) = 1 + I where I^2 = -1, C(n+1) =
 *          1/(1+C(n)); then a(n) = (-1)^n/Im(C(n)) where Im(z) is the
 *          imaginary part of the complex number z.
 * https://oeis.org/A069921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A069921
   a(0) = 1, a(1) = 5, a(2) = 10
   a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3)   for n >= 3
   Recurrence uses only +, -, * on seq_t values. */

#ifndef SEQUENCE_A069921_H
#define SEQUENCE_A069921_H

#include <stdint.h>

/* The sequence entry function. */
seq_t a(idx_t n)
{
    /* Return the first few values directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)10;

    /* Use three rolling variables to compute the recurrence. */
    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)5;   /* a(n-2) */
    seq_t a2 = (seq_t)10;  /* a(n-1) */
    seq_t an;              /* a(n) */

    /* Compute iteratively up to the desired index. */
    for (idx_t i = 3; i <= n; ++i)
    {
        an = 2*a2 + 2*a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }

    return an;
}

#endif /* SEQUENCE_A069921_H */
