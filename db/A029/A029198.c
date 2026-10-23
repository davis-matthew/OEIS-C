/*
 * A029198  Expansion of 1/((1-x^2)*(1-x^5)*(1-x^6)*(1-x^12)).
 * https://oeis.org/A029198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^3+45*n^2+604*n+3984)/4320 - (n^2+25*n+100)*(n
 *            mod 2)/288 + (n+15)*(((n+5) mod 6)-((n+1) mod 6))/216)
 * Range:     u8 n <= 89, u16 n <= 641, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* use signed 128‑bit arithmetic for the intermediate numerator */
    __int128 N   = (__int128)n;
    __int128 n2  = N * N;
    __int128 n3  = n2 * N;

    __int128 term1 = n3
                    + 45 * n2
                    + 604 * N
                    + 3984;

    __int128 n2p25Np100 = n2 + 25 * N + 100;
    __int128 term2 = n2p25Np100 * ( (__int128)(n & 1) );   /* (n mod 2) */

    /* difference of the two mod‑6 residues (may be negative) */
    __int128 diff = ( (__int128)((n + 5) % 6) )
                  - ( (__int128)((n + 1) % 6) );

    __int128 term3 = (N + 15) * diff;

    __int128 num = term1
                   - 15 * term2
                   + 20 * term3;

    /* integer division truncates towards zero; for non‑negative
       numbers this is equivalent to floor. */
    return (seq_t)(num / 4320);
}
