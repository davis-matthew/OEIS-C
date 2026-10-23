/*
 * A125255  Smallest prime divisor of 4n-1.
 * https://oeis.org/A125255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest prime divisor of 4*n - 1 (OEIS A125255)
 * Range:     u8 n <= 65, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* OEIS offset is 1; values for n < 1 are undefined, return 0 */
    if (n < 1) return 0;

    /* 4*n-1 fits in 64‑bit for the ranges used (n ≤ 10 000). */
    uint64_t m = 4 * (uint64_t)n - 1;

    /* 4*n-1 is always odd, so start checking from 3. */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) return (seq_t)p;
    }

    /* m itself is prime. */
    return (seq_t)m;
}
