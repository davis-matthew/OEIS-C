/*
 * A059600  Expansion of (1+6*x+x^2)/(1-x)^8.
 * https://oeis.org/A059600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+5,5)*(4*n^2+24*n+21)/21
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute (4*n^2 + 24*n + 21) */
    seq_t poly = (seq_t)4 * n * n + (seq_t)24 * n + 21;

    /* Compute product of (n+5)*(n+4)*(n+3)*(n+2)*(n+1)*poly
       and divide by 120*21 = 2520. Use unsigned __int128 to avoid overflow
       for typical n up to at least 1e6. */
    unsigned __int128 prod = 1;
    prod *= (unsigned __int128)(n + 5);
    prod *= (unsigned __int128)(n + 4);
    prod *= (unsigned __int128)(n + 3);
    prod *= (unsigned __int128)(n + 2);
    prod *= (unsigned __int128)(n + 1);
    prod *= (unsigned __int128)poly;

    prod /= 2520;           /* exact division */

    return (seq_t)prod;
}
