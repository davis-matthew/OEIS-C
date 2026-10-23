/*
 * A008629  Molien series for A_6.
 * https://oeis.org/A008629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = floor((3*n^5+45*n^4+1730*n^3+13950*n^2+56040*n+129600)/129600
 *                - (n mod 2)*n*5/64 + ((2*n^2+1) mod 3)*n/27)
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    const seq_t D = (seq_t)129600;          // common denominator
    const seq_t D_over_64 = (seq_t)(D / 64); // 2025
    const seq_t D_over_27 = (seq_t)(D / 27); // 4800

    seq_t n_seq = (seq_t)n;

    /* compute powers of n */
    seq_t n2 = n_seq * n_seq;             // n^2
    seq_t n3 = n2 * n_seq;                // n^3
    seq_t n4 = n3 * n_seq;                // n^4
    seq_t n5 = n4 * n_seq;                // n^5

    /* numerator of the main term */
    seq_t A = (seq_t)3 * n5
            + (seq_t)45 * n4
            + (seq_t)1730 * n3
            + (seq_t)13950 * n2
            + (seq_t)56040 * n_seq
            + (seq_t)129600;              // constant term

    /* adjustment terms */
    seq_t B = ((n_seq % (seq_t)2) * n_seq * (seq_t)5);                // (n mod 2)*n*5
    seq_t C = (((((seq_t)2 * n2) + (seq_t)1) % (seq_t)3) * n_seq);   // ((2*n^2+1) mod 3)*n

    /* combine with common denominator and take floor */
    seq_t S = A - B * D_over_64 + C * D_over_27;
    return S / D;
}
