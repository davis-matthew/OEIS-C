/*
 * A055796  T(2n+3,n), array T as in A055794.
 * https://oeis.org/A055796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+2)*(n+3)*(n+4)*(n^2-n+30)/720
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 119, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use unsigned __int128 for intermediate calculations to avoid overflow. */
    unsigned __int128 prod = (unsigned __int128)n + 1;
    prod *= (unsigned __int128)n + 2;
    prod *= (unsigned __int128)n + 3;
    prod *= (unsigned __int128)n + 4;
    unsigned __int128 extra = (unsigned __int128)n * n - (unsigned __int128)n + 30;
    prod *= extra;
    prod /= 720;
    return (seq_t)prod;
}
