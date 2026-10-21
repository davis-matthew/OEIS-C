/*
 * A225081  Gray code variant of A048896.
 * https://oeis.org/A225081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(A005811(n+1)-1)
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* convert n+1 to seq_t and compute Gray code weight */
    seq_t x = ((seq_t)n) + 1;
    x ^= x >> 1;                  /* x ^= x >> 1  transforms to Gray code weight */

    idx_t wt = 0;                  /* number of 1‑bits in the Gray code */
    while (x) {
        wt += (idx_t)(x & 1);
        x >>= 1;
    }

    /* wt is at least 1 for all n >= 0, so wt-1 is safe */
    return pow2(wt - 1);
}
