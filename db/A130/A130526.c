/*
 * A130526  A permutation of the integers induced by the lower and upper
 *          Wythoff sequences.
 * https://oeis.org/A130526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = k if n = floor(k*φ) (lower Wythoff), a(n) = -k if n = k +
 *            floor(k*φ) (upper Wythoff)
 * Range:     i8 n <= 68, i16 n <= 68, i32 n <= 68, i64 n <= 68, i128 n <= 68,
 *            big any n
 * Helpers:   floor_k_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_k_phi.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    for (idx_t k = 1;; ++k)
    {
        seq_t l = floor_k_phi(k);          /* lower Wythoff term  */
        if ((seq_t)n == l) return (seq_t)k;

        seq_t u = l + (seq_t)k;            /* upper Wythoff term  */
        if ((seq_t)n == u) return -(seq_t)k;

        /* Both sequences have surpassed n → n must have been found already */
        if (l > n && u > n) return 0;
    }
}
