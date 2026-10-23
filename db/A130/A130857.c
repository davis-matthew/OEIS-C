/*
 * A130857  a(n) = (n-1)*n*(n+1)*(n+2)*(2*n+11)/120.
 * https://oeis.org/A130857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*n*(n+1)*(n+2)*(2*n+11)/120
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* below offset; behaviour irrelevant */
    /* Use a wider type for intermediate calculations to avoid overflow. */
    unsigned __int128 prod = 1;
    prod *= (unsigned __int128)(n - 1);
    prod *= (unsigned __int128)(n);
    prod *= (unsigned __int128)(n + 1);
    prod *= (unsigned __int128)(n + 2);
    prod *= ((unsigned __int128)2 * n + 11);
    prod /= 120;
    return (seq_t)prod;
}
