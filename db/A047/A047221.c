/*
 * A047221  Numbers that are congruent to {2, 3} mod 5.
 * https://oeis.org/A047221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*⌊(n-1)/2⌋ + (n mod 2 ? 2 : 3)
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A047221 (offset 1) */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for indices < 1 */
    seq_t k = (seq_t)((n - 1) / 2); /* integer division */
    seq_t val = 5 * k + ((n & 1) ? 2 : 3);
    return val;
}
