/*
 * A125758  Numbers congruent to 4 or 7 (mod 9).
 * https://oeis.org/A125758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = a(n-2) + 9   for n>=3
 * Initial terms: a(1)=4, a(2)=7
 * Closed form: a(n) = 9*floor((n-1)/2) + (n%2 ? 4 : 7)
 * This implements the sequence of numbers congruent to 4 or 7 (mod 9).
 */
#include <stdint.h>

seq_t a(idx_t n) {
    /* n is 1‑based; assume n >= 1 */
    seq_t block = (seq_t)(n - 1) / 2;          /* floor((n-1)/2)   */
    seq_t offset = ((n & 1ULL) ? 4 : 7);       /* 4 for odd n, 7 for even n   */
    return 9 * block + offset;
}
