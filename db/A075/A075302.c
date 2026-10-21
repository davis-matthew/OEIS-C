/*
 * A075302  Transpose of array A075300.
 * https://oeis.org/A075302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (2^i) * (2*j + 1) – 1 * where i = n – (s*(s+1))/2, j = s
 *            – i, and s = ⌊(√(8n+1)−1)/2⌋. * This gives the element on the
 *            anti‑diagonal with sum s, ordered by * increasing i
 * Range:     u8 n <= 41, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   pow_seq, tri_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* for size_t */
#include <stdint.h>          /* for uint64_t if needed */
#include "../utils/pow_seq.h"/* pow_seq(seq_t base, int k) */
#include "../utils/tri_idx.h"

seq_t a(idx_t n) {
    /* Find s such that T(s) > n and T(s-1) <= n. */
    idx_t s = 0;
    while (tri_idx(s) <= n) s++;

    /* offset within diagonal s. */
    idx_t offset = n - tri_idx(s - 1);        /* s*(s+1)/2 */
    idx_t i = offset;
    idx_t j = s - offset;

    /* Compute 2^i. */
    seq_t pow2 = pow_seq((seq_t)2, (int)i);   /* uses wrapper for arbitrary width */
    /* a = 2^i * (2*j + 1) – 1 */
    seq_t result = pow2 * (seq_t)(2 * j + 1) - 1;
    return result;
}
