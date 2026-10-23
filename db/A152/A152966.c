/*
 * A152966  Twice repdigit numbers.
 * https://oeis.org/A152966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 2*A010785(n) (twice the nth repdigit; A010785(0)=0)
 * Range:     u8 n <= 19, u16 n <= 38, u32 n <= 82, u64 n <= 170,
 *            u128 n <= 343, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the nth term of OEIS sequence A152966 */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;                /* a(0) = 0 */

    /* Determine the length (number of digits) and the repeated digit */
    idx_t m   = n - (idx_t)1;            /* shift to zero‑based after the zero term */
    idx_t d   = m / (idx_t)9 + (idx_t)1; /* digit length (>=1) */
    seq_t digit = (seq_t)((m % (idx_t)9) + (idx_t)1); /* repeat digit (1..9) */

    /* Compute the repunit of length d: 1, 11, 111, ... */
    seq_t repunit = (seq_t)0;
    for (idx_t i = (idx_t)0; i < d; ++i)
        repunit = repunit * (seq_t)10 + (seq_t)1;

    /* Repdigit = digit * repunit, then double it */
    seq_t repdigit = repunit * digit;
    return repdigit * (seq_t)2;
}
