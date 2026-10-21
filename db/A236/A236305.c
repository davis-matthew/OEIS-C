/*
 * A236305  The number of P-positions in the game of Nim with up to 3 piles,
 *          allowing for piles of zero, such that the number of objects in
 *          each pile does not exceed n.
 * https://oeis.org/A236305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2*floor(log₂(n))) + 3*(n + 1 - 2^floor(log₂(n)))^2
 *            (for n ≥ 1); a(0) = 1
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A236305 for any idx_t n (uint64_t).
   No external libraries are required beyond the standard C headers. */
#include <stdint.h> /* for uint64_t */

seq_t a(idx_t n) {
    /* Special case n = 0 */
    if (n == 0) return (seq_t)1;

    /* Compute b = floor(log₂(n)) by shifting */
    unsigned b = 0;
    idx_t temp = n;
    while (temp >>= 1) ++b;          /* b = floor(log2(n)) */

    /* Compute 2^(2*b)    == 1 << (2*b)   */
    seq_t pow4 = (seq_t)1 << (2 * b);

    /* Compute 2^b */
    seq_t pow2b = (seq_t)1 << b;

    /* Compute c = n + 1 - 2^b */
    seq_t c = (seq_t)(n + 1) - pow2b;

    /* Return pow4 + 3*c^2 */
    seq_t c2 = c * c;
    return pow4 + (seq_t)3 * c2;
}
