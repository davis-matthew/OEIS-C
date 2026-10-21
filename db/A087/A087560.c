/*
 * A087560  Smallest m > n such that gcd(m, n^2) = n.
 * https://oeis.org/A087560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * (smallest integer > 1 coprime to n)
 * Range:     u8 n <= 53, u16 n <= 6089, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euclidean algorithm for idx_t (unsigned 64‑bit integer). */
static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    /* Find the smallest k > 1 such that gcd(k, n) == 1. */
    idx_t k = 2;
    while (gcd_idx(k, n) != 1) {
        ++k;
    }
    /* Return the product in seq_t type. */
    return (seq_t)n * (seq_t)k;
}
