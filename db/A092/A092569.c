/*
 * A092569  Permutation of integers a(a(n)) = n. In binary representation of
 *          n, transformation of inner bits, 1 <-> 0, gives binary
 *          representation of a(n).
 * https://oeis.org/A092569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n XOR ((1 << ⌊log₂ n⌋) – 2) (for n ≥ 2) a(0) = 0, a(1) =
 *            1. The “inner bits” of n are all bits except the
 *            most‑significant and least‑significant bits; they are inverted.
 *            This implementation uses O(1) arithmetic on the machine word
 *            size
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 2)
        return (seq_t)n;
    /* Find position k of the highest set bit (0‑based). */
    idx_t k = 63 - __builtin_clzll((unsigned long long)n); /* works for all n ≥ 2 */
    /* Build mask with 1’s on all positions 1 … k‑1. */
    seq_t mask = ((seq_t)1 << k) - 2;
    return (seq_t)n ^ mask;
}
