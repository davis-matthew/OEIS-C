/*
 * A059602  Expansion of (5+10*x+x^2)/(1-x)^10.
 * https://oeis.org/A059602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+7,7)*(4*n^2+41*n+90)/18
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute binomial(n+7,7) as (n+1)*(n+2)*...*(n+7)/7! */
    unsigned __int128 prod = 1;
    for(idx_t i = 1; i <= 7; ++i) {
        prod *= (unsigned __int128)(n + i);
    }
    prod /= 5040;                /* 7! = 5040 */

    unsigned __int128 bin   = prod;        /* binomial value as 128‑bit */
    unsigned __int128 nn128 = (unsigned __int128)n;

    unsigned __int128 quad = 4 * nn128 * nn128 + 41 * nn128 + 90;
    unsigned __int128 res  = bin * quad;
    res /= 18;                 /* final division */

    return (seq_t)res;
}
