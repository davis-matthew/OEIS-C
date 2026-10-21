/*
 * A206479  Number of terms common to the binary expansions of m and n; a
 *          matrix by antidiagonals.
 * https://oeis.org/A206479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   a(n) = popcount(i & j) where (i,j) are antidiagonal coordinates
 *            of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    if (n == 0u)
        return 0;
    idx_t s = 1;
    /* Find the smallest s such that s*(s-1)/2 >= n.
       Use 128‑bit arithmetic to avoid overflow. */
    while (((__uint128_t)s * (s - 1)) < ((__uint128_t)n << 1))
        ++s;

    /* Previous cumulative count before diagonal s */
    __uint128_t tmp = (__uint128_t)(s - 2) * (s - 1);
    idx_t prev = (idx_t)(tmp >> 1);  /* tmp / 2 */

    /* Position on diagonal: 1‑based */
    idx_t i = n - prev;
    idx_t j = s - i;

    /* Popcount of the bitwise AND of i and j */
    return (seq_t)__builtin_popcountll(i & j);
}
