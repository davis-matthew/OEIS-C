/*
 * A006689  Number of deterministic, completely-defined, initially-connected
 *          finite automata with 2 inputs and n unlabeled states.
 * https://oeis.org/A006689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum (Product_{i=1..n-1} i^(f_i - f_{i-1} - 1)) * n^(2*n -
 *            f_{n-1} - 1), sum over 0 <= f_1 < 2 and f_{i-1} < f_i < 2*i for
 *            i = 2..n-1; a(1) = 1. Evaluated by dynamic programming over (i,
 *            f_i), using only additions and multiplications
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;

    idx_t len = 2 * n;
    seq_t *D = alloc_zero(len);
    /* i = 1: f_1 in {0, 1}, the factor is 1^(...) = 1 */
    D[0] = 1;
    D[1] = 1;

    for (idx_t i = 2; i + 1 <= n; i++) {
        /* new D[f] = Sum_{f' < f} oldD[f'] * i^(f - f' - 1), for f < 2i */
        seq_t S = 0;
        idx_t lim = 2 * i;
        for (idx_t f = 0; f < lim; f++) {
            seq_t t = D[f];
            D[f] = S;
            S = S * (seq_t)i + t;
        }
    }

    /* result = Sum_f D[f] * n^(2n - 1 - f), f = 0..2n-3 (Horner), times n^2 */
    seq_t acc = 0;
    for (idx_t f = 0; f + 3 <= len; f++) {
        acc = acc * (seq_t)n + D[f];
    }
    acc = acc * (seq_t)n * (seq_t)n;

    release(D, len);
    return acc;
}
