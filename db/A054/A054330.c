/*
 * A054330  One half of sixth unsigned column of Lanczos' triangle A053125.
 * https://oeis.org/A054330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(2*n-1) * binomial(2*n+6, 5) (for n >= 1) a(0) = 3
 *            Binomial is computed multiplicatively to avoid overflow where
 *            possible
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 8, u64 n <= 21, u128 n <= 51,
 *            big any n
 * Helpers:   binom_5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom_5.h"

seq_t a(idx_t n) {
    if (n == 0) {            /* handle the offset explicitly */
        return (seq_t)3;
    }

    /* 2^(2*n-1) : compute by left‑shifting 1, (2*n-1) times. */
    seq_t pow2 = (seq_t)1;
    idx_t shift = 2 * n - 1;
    for (idx_t i = 0; i < shift; ++i) {
        pow2 <<= 1;          /* safe even if seq_t is 64‑bit; overflow wraps */
    }

    /* Binomial factor */
    seq_t bin = binom_5((idx_t)(2 * n + 6));

    return pow2 * bin;
}
