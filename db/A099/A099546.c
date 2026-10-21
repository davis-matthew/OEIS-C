/*
 * A099546  Odd part of n modulo 5.
 * https://oeis.org/A099546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (odd part of n) mod 5 That is, let m be the largest odd
 *            divisor of n, then a(n) = m % 5
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute the odd part of n using 64‑bit arithmetic to avoid overflow
       when seq_t may be smaller than 64 bits.  The result is always < 5,
       so it safely fits into seq_t. */
    uint64_t m = n;
    while ((m & 1ULL) == 0ULL)  /* remove factors of 2 */
        m >>= 1ULL;
    return (seq_t)(m % 5ULL);
}
