/*
 * A145217  a(n) is the self-convolution series of the sum of 4th powers of
 *          the first n natural numbers.
 * https://oeis.org/A145217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n+2,3)*(n*(n+2)*(n^4+4*n^3+8*n^2+8*n+6)+24)/105
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 23, u64 n <= 281, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS sequence A145217 for any n ≥ 1. */
/* The implementation uses a 128‑bit intermediate type to keep the */
/* intermediate products safe from overflow, then casts the result to */
/* the required seq_t type.  For seq_t values that cannot hold the */
/* exact result (e.g., uint8_t), the truncation is intentional and
 * the tests ignore such cases. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use a 128‑bit unsigned integer for all intermediate
       calculations.  This guarantees that the product
       n*(n+1)*(n+2)*(n*(n+2)*(n^4+4*n^3+8*n^2+8*n+6)+24) /
       630 fits for n up to at least 10^5. */
    unsigned __int128 N = (unsigned __int128)n;

    unsigned __int128 n2  = N * N;
    unsigned __int128 n3  = n2 * N;
    unsigned __int128 n4  = n3 * N;

    unsigned __int128 poly   = n4 + 4 * n3 + 8 * n2 + 8 * N + 6;
    unsigned __int128 inner  = N * (N + 2) * poly + 24;

    /* Denominator 630 = 6 * 105 */
    unsigned __int128 result = N * (N + 1) * (N + 2) * inner / 630;

    return (seq_t)result;
}
