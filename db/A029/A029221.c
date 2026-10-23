/*
 * A029221  Expansion of 1/((1-x^2)*(1-x^6)*(1-x^9)*(1-x^10)).
 * https://oeis.org/A029221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^3+54*n^2+856*n+6480)/6480 - (n^2+27*n+133)*(n
 *            mod 2)/240 + n*(((n+2) mod 3) - (n mod 3))/162)
 * Range:     u8 n <= 101, u16 n <= 733, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the number of partitions of n into parts 2, 6, 9, and 10. */
seq_t a(idx_t n)
{
    /* Use a 128‑bit signed integer for the intermediate calculations. */
    __int128 N   = ( __int128 )n;                    /* cast standard index to 128‑bit */
    __int128 N2  = N * N;                            /* n^2 */
    __int128 N3  = N2 * N;                           /* n^3 */

    /* First term numerator: n^3 + 54*n^2 + 856*n + 6480 */
    __int128 A = N3 + (__int128)54 * N2 + (__int128)856 * N + 6480;

    /* Second term: (n^2 + 27*n + 133) * (n mod 2) */
    __int128 B = (N2 + (__int128)27 * N + 133) *
                 ( (idx_t)n % 2 );   /* 0 or 1, no overflow */

    /* Third term: n * ( ((n+2) mod 3) - (n mod 3) ) */
    int diff = (int)(((idx_t)n + 2) % 3) - (int)((idx_t)n % 3); /* -2 .. 2 */
    __int128 C = N * ( __int128 )diff;

    /* Combine over common denominator 6480:
        (A)/6480 - (B)/240 + (C)/162
        -> (A - 27*B + 40*C) / 6480    (240 = 6480/27, 162 = 6480/40)
    */
    __int128 numerator = A - (__int128)27 * B + (__int128)40 * C;

    /* Floor division – in unsigned arithmetic truncation is already a floor. */
    seq_t result = (seq_t)( numerator / 6480 );

    return result;
}
