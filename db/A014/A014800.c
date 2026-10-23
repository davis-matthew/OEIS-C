/*
 * A014800  Squares of even pentagonal pyramidal numbers.
 * https://oeis.org/A014800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0; For n >= 1 let m = 4*((n-1)/3) + 2 + ((n-1)%3); Then
 *            a(n) = (m^2 * (m+1) / 2)^2
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 37, u64 n <= 1500,
 *            u128 n <= 1500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes squares of even pentagonal pyramidal numbers (OEIS A014800). */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Compute m = 4*((n-1)/3) + 2 + ((n-1)%3) */
    idx_t q = (n - 1) / 3;        /* floor((n-1)/3)  */
    idx_t r = (n - 1) % 3;        /* (n-1) mod 3      */
    idx_t m = 4 * q + 2 + r;      /* 4*q + (r+2)      */

    /* Convert to seq_t for calculations */
    seq_t ms = (seq_t)m;
    seq_t val = ms * ms * (ms + 1) / 2; /* m^2 * (m+1) / 2 */
    return val * val;                  /* square the result   */
}
