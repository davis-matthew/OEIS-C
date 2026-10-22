/*
 * A000011  Number of n-bead necklaces (turning over is allowed) where
 *          complements are equivalent.
 * https://oeis.org/A000011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (A000013(n) + 2^floor(n/2))/2, with A000013(n) = (1/(2n))
 *            * Sum_{d|n} phi(2d)*2^(n/d) for n >= 1, a(0) = 1. So a(n) =
 *            (Sum_{d|n} phi(2d)*2^(n/d) + 2n*2^floor(n/2)) / (4n). Each term
 *            is kept as quotient/remainder mod 4n so intermediates never
 *            exceed a(n)
 * Range:     u8 n <= 13, u16 n <= 22, u32 n <= 39, u64 n <= 72, u128 n <= 137,
 *            big any n
 * Helpers:   add_term, phi_ull
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/add_term.h"
#include "../utils/phi_ull.h"

seq_t a(idx_t n)
{
    unsigned long long nn, M, d;
    seq_t Q = 0;
    unsigned long long R = 0;
    if (n == 0) return (seq_t)1;
    nn = (unsigned long long)n;
    M = 4 * nn;
    for (d = 1; d * d <= nn; d++) {
        if (nn % d == 0) {
            unsigned long long e = nn / d;
            add_term(&Q, &R, 1, e, phi_ull(2 * d), M);
            if (e != d)
                add_term(&Q, &R, 1, d, phi_ull(2 * e), M);
        }
    }
    add_term(&Q, &R, 2 * nn, nn / 2, 1, M);
    return Q;
}
