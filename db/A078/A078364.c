/*
 * A078364  A Chebyshev S-sequence with Diophantine property.
 * https://oeis.org/A078364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n-1) - a(n-2), with a(0) = 1, a(1) = 15 (OEIS
 *            A078364)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef A078364_H
#define A078364_H

/* The function implements the linear recurrence directly.
   It works for any build in which seq_t supports the usual
   integer arithmetic operators (+, -, *). */

seq_t a(idx_t n)
{
    /* Handle the two base cases explicitly. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)15;

    /* Iterative computation of the recurrence. */
    seq_t a_nm2 = (seq_t)1;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)15;  /* a(n-1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t temp = a_nm1;
        a_nm1 = (seq_t)15 * a_nm1 - a_nm2;
        a_nm2 = temp;
    }
    return a_nm1;
}

#endif  /* A078364_H */
