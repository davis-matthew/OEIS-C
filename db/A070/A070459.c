/*
 * A070459  a(n) = n^2 mod 37.
 * https://oeis.org/A070459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 mod 37
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Compute n modulo 37, square it using a 64‑bit intermediate,
       then take the result modulo 37 again.  The value fits in
       seq_t for any build. */
    uint64_t r = n % 37ULL;
    uint64_t temp = r * r;
    return (seq_t)(temp % 37ULL);
}
