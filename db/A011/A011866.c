/*
 * A011866  a(n) = floor(n*(n-1)/13).
 * https://oeis.org/A011866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)/13)
 * Range:     u8 n <= 58, u16 n <= 923, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;
#ifdef __SIZEOF_INT128__
    /* Use 128‑bit intermediate to avoid overflow for 64‑bit seq_t. */
    __uint128_t prod = (__uint128_t)n * ((__uint128_t)n - 1U);
    prod /= 13;
    return (seq_t)prod;
#else
    /* Fallback: rely on seq_t multiplying without overflow.  For
       practical n this is safe; the 64‑bit build should enable
       __uint128_t on the common compilers. */
    seq_t N = (seq_t)n;
    return (N * (N - 1)) / 13;
#endif
}
