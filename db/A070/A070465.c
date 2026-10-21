/*
 * A070465  a(n) = n^2 mod 43.
 * https://oeis.org/A070465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 mod 43
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* reduce n modulo 43 first to avoid overflow */
    unsigned long long k = n % 43ULL;
    unsigned long long prod = k * k;
    return (seq_t)(prod % 43ULL);
}
