/*
 * A100802  a(n) = least k >= 0 such that (n+k)/2 is prime.
 * https://oeis.org/A100802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = min{k ≥ 0 : (n+k)/2 is prime}
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Simple primality test for 64‑bit unsigned integers. */
static inline bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    if (x == 2 || x == 3) return true;
    if ((x & 1) == 0) return false;
    for (uint64_t d = 3; d * d <= x; d += 2) {
        if (x % d == 0) return false;
    }
    return true;
}

/* Computes a(n) */
seq_t a(idx_t n)
{
    /* Start searching from the smallest integer m >= ceil(n/2). */
    idx_t m_start = (n + 1) / 2;          /* ceil(n/2) */
    for (uint64_t m = m_start;; ++m) {
        if (is_prime_uint64(m)) {
            idx_t k = (idx_t)(2 * m - n); /* k = 2*m - n  (k >= 0) */
            return (seq_t)k;
        }
    }
}
