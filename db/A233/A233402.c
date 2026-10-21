/*
 * A233402  Number of (n+1) X (1+1) 0..2 arrays with row and column sums
 *          nondecreasing, and no adjacent elements equal.
 * https://oeis.org/A233402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^2*(11-(-1)^n) + n*(50-6*(-1)^n) + (62-14*(-1)^n)) / 16
 * Range:     u8 n <= 2, u16 n <= 83, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-2) - 3*a(n-4) + a(n-6) with a(1)=9, a(2)=11, a(3)=22, a(4)=24, a(5)=41, a(6)=42. */
/* The implementation adopts the closed-form expression, which is O(1) and exact. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* safety for out-of-bounds call */

    /* Choose coefficients depending on parity of n */
    const bool odd = (n & 1);
    const seq_t coeff_n2 = odd ? (seq_t)12 : (seq_t)10;   /* 11-(-1)^n */
    const seq_t coeff_n  = odd ? (seq_t)56 : (seq_t)44;   /* 50-6*(-1)^n */
    const seq_t coeff_1  = odd ? (seq_t)76 : (seq_t)48;   /* 62-14*(-1)^n */

    const seq_t ns = (seq_t)n;
    const seq_t nsq = ns * ns;

    const seq_t numer = nsq * coeff_n2 + ns * coeff_n + coeff_1;

    return numer / (seq_t)16;
}
