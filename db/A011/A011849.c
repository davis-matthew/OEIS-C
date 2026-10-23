/*
 * A011849  a(n) = floor(binomial(n,3)/3).
 * https://oeis.org/A011849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(binomial(n,3)/3) = floor(n*(n-1)*(n-2)/18)
 * Range:     u8 n <= 17, u16 n <= 106, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3U) return (seq_t)0;
    /* use unsigned __int128 for safe intermediate product */
    unsigned __int128 prod = (unsigned __int128)n;
    prod *= (n - 1U);
    prod *= (n - 2U);
    prod /= (unsigned __int128)18U;
    return (seq_t)prod;
}
