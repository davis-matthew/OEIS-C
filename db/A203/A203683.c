/*
 * A203683  Vandermonde sequence using x^2 + y^2 applied to
 *          (1,2,4,...,2^(n-1)).
 * https://oeis.org/A203683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^{n(n-1)(n-2)/3} * ∏_{k=1}^{n-1} (1 + 2^{2k})^{\,n-k}
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   pow2, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived from the Vandermonde determinant of (x_i^2 + x_j^2) with x_i = 2^{i-1}  */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow2.h"      /* pow2(idx_t exp) -> seq_t  */
#include "../utils/pow_seq.h"   /* pow_seq(seq_t base, int exp) -> seq_t  */

seq_t a(idx_t n) {
    /* The first term is defined as 1. */
    if (n == 1) {
        return (seq_t)1;
    }

    seq_t result = (seq_t)1;

    /* Multiply the product part: */
    for (idx_t k = 1; k < n; ++k) {
        seq_t base = pow2(2 * k);   /* 2^{2k}  */
        base += (seq_t)1;           /* 1 + 2^{2k}  */
        seq_t term = pow_seq(base, (int)(n - k)); /* exponent fits in int */
        result *= term;
    }

    /* Multiply the power of 2 factor: exponent = n(n-1)(n-2)/3  */
    __int128 mult = (__int128)n * (n - 1) * (n - 2);
    idx_t exp = (idx_t)(mult / 3);
    result *= pow2(exp);

    return result;
}
