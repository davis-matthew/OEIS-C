/*
 * A051489  a(n) = n^(n+2) + (n+2)^n.
 * https://oeis.org/A051489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n+2) + (n+2)^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   pow_seq_expo
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq_expo.h"

seq_t a(idx_t n) {
    seq_t ns = (seq_t)n;
    seq_t n_plus_2 = ns + (seq_t)2;

    /* Term 1: n^(n+2) */
    seq_t term1 = pow_seq_expo(ns, n + (idx_t)2);

    /* Term 2: (n+2)^n */
    seq_t term2 = pow_seq_expo(n_plus_2, n);

    return term1 + term2;
}
