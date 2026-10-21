/*
 * A051946  Expansion of g.f.: (1+4*x)/(1-x)^7.
 * https://oeis.org/A051946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+5,5)*(5*n+6)/6
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 89, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          a(n) = (n+1)*(n+2)*(n+3)*(n+4)*(n+5)*(5*n+6)/720 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use a 128‑bit intermediate to avoid overflow on common
       compilers that support unsigned __int128. The final division
       by 720 is exact as the sequence is integer‑valued. */

    unsigned __int128 prod = 1;
    prod *= (unsigned __int128)(n + 5);
    prod *= (unsigned __int128)(n + 4);
    prod *= (unsigned __int128)(n + 3);
    prod *= (unsigned __int128)(n + 2);
    prod *= (unsigned __int128)(n + 1);
    prod *= ((unsigned __int128)5 * n + 6);
    prod /= (unsigned __int128)720;

    return (seq_t)prod;
}
