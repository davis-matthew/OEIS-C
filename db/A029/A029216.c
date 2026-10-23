/*
 * A029216  Expansion of 1/((1-x^2)*(1-x^6)*(1-x^7)*(1-x^10)).
 * https://oeis.org/A029216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^3+48*n^2+684*n+3600)/5040 * - ((n^2+25*n+42)*(n
 *            mod 2))/240 * + ((n^3+6*n^2+5*n+2) mod 7)/7)
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* convert index to seq_t */
    seq_t N = (seq_t)n;
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;

    /* term A : (n^3+48*n^2+684*n+3600) */
    seq_t termA = n3 + (seq_t)48 * n2 + (seq_t)684 * N + (seq_t)3600;

    /* term B : 21 * ((n^2+25*n+42)*(n mod 2)) */
    seq_t partB = (n2 + (seq_t)25 * N + (seq_t)42) *
                  ( (n & (idx_t)1) ? (seq_t)1 : (seq_t)0 );
    seq_t termB = (seq_t)21 * partB;

    /* term C : 720 * ((n^3+6*n^2+5*n+2) mod 7) */
    seq_t rem = (n3 + (seq_t)6 * n2 + (seq_t)5 * N + (seq_t)2) % (seq_t)7;
    seq_t termC = (seq_t)720 * rem;

    /* combine with common denominator 5040 */
    seq_t numer = termA - termB + termC;
    return numer / (seq_t)5040;
}
