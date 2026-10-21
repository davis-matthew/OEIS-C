/*
 * A064875  Third of four sequences representing the lexicographical minimal
 *          decomposition of n in four squares: n = A064873(n)^2 +
 *          A064874(n)^2 + a(n)^2 + A064876(n)^2.
 * https://oeis.org/A064875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the third component of the lexicographically minimal
 *            decomposition of n into four squares: n = A064873(n)^2 +
 *            A064874(n)^2 + a(n)^2 + A064876(n)^2
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n) {
    /* Convert n to seq_t for arithmetic */
    seq_t nval = (seq_t)n;

    /* Loop over the first component a in increasing order */
    idx_t maxA = _floor_isqrt_u64(n);
    for (idx_t a = 0; a <= maxA; ++a) {
        seq_t a2 = (seq_t)a * a;
        seq_t remA = nval - a2;

        /* Second component */
        idx_t maxB = _floor_isqrt_u64((uint64_t)remA);
        for (idx_t b = 0; b <= maxB; ++b) {
            seq_t b2 = (seq_t)b * b;
            seq_t remB = remA - b2;

            /* Third component */
            idx_t maxC = _floor_isqrt_u64((uint64_t)remB);
            for (idx_t c = 0; c <= maxC; ++c) {
                seq_t c2 = (seq_t)c * c;
                seq_t remC = remB - c2;

                /* Fourth component: check if remC is a perfect square */
                idx_t d = _floor_isqrt_u64((uint64_t)remC);
                if ((seq_t)d * d == remC) {
                    /* Lexicographically smallest quadruple found;
                       return its third component */
                    return (seq_t)c;
                }
            }
        }
    }

    /* In theory this point is never reached (Lagrange's theorem guarantees
       a representation in four squares for every n).  Return 0 as a safe
       fallback. */
    return 0;
}
