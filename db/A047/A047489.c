/*
 * A047489  Numbers that are congruent to {1, 2, 3, 5, 7} mod 8.
 * https://oeis.org/A047489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 * ((n-1)/5) + {1,2,3,5,7}[(n-1) mod 5]
 * Range:     u8 n <= 160, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t _a047489_offsets[5] = {1, 2, 3, 5, 7};

seq_t a(idx_t n) {
    /* Assumes n ≥ 1 */
    idx_t m = n - 1;
    idx_t q = m / 5;
    idx_t r = m % 5;
    return (seq_t)8 * (seq_t)q + _a047489_offsets[(size_t)r];
}
