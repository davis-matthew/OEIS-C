/*
 * A035876  Coordination sequence for A_50 lattice.
 * https://oeis.org/A035876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d=1}^{50} C(51,d) * C(n-1,d-1) * C(50-d+n,n) (n >=
 *            1, a(0)=1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 12 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

static inline seq_t mult_safe(seq_t a, seq_t b) {
#if defined(__GNUC__) || defined(__clang__)
    /* Use 128‑bit arithmetic when seq_t is 64‑bit to avoid overflow */
    if (sizeof(seq_t) <= 8) {
        return (seq_t)(((__int128)a) * (__int128)b);
    } else {
        return a * b;
    }
#else
    /* Fallback: rely on seq_t's built‑in multiplication (works for big‑int) */
    return a * b;
#endif
}

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t sum = (seq_t)0;
    idx_t maxd = (n < 50) ? n : 50;

    for (idx_t d = 1; d <= maxd; ++d) {
        seq_t term1 = binomial((idx_t)51, d);
        seq_t term2 = binomial(n - 1, d - 1);
        seq_t term3 = binomial(50 - d + n, n);

        seq_t temp = mult_safe(term1, term2);
        temp = mult_safe(temp, term3);
        sum += temp;
    }

    return sum;
}
