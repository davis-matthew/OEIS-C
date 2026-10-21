/*
 * A219392  Numbers k such that 22*k+1 is a square.
 * https://oeis.org/A219392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 109, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A219392: Numbers k such that 22*k+1 is a square
   Formula: a(n) = (22*n*(n-1) + 9*(-1)^n*(2*n-1) + 1)/4 + 2   (n ≥ 1)
   (All calculations are performed in signed __int128 to avoid overflow.) */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* safety for potential 0 index */
    __int128 N = n;                       /* cast to 128‑bit signed integer */
    __int128 term = 22 * N * (N - 1);      /* 22*n*(n-1) */
    __int128 two_n_minus1 = 2 * N - 1;     /* 2*n-1          */
    __int128 sign = (n & 1) ? -1 : 1;      /* (-1)^n         */
    term += 9 * sign * two_n_minus1;       /* 9*(-1)^n*(2n-1) */
    term += 1;                             /* +1              */
    term /= 4;                             /* divide by 4     */
    term += 2;                             /* +2              */
    return (seq_t)term;                    /* cast back to seq_t */
}
