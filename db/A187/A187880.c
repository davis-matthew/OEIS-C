/*
 * A187880  Number of n X n matrices over GF(2) that can be used as a kernel
 *          to construct a polar code. That is, the number of matrices for
 *          which channel polarization occurs.
 * https://oeis.org/A187880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   factorial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A187880: Number of n×n matrices over GF(2) that can be used as a
   kernel to construct a polar code.
   Formula: a(n) = Π_{i=0}^{n-1} (2^n - 2^i) - n! * 2^{n*(n-1)/2} */

#include "../utils/pow2.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == 0)      /* offset is 1, but guard against 0 */
        return 0;

    /* |GL(n,2)| = Π_{i=0}^{n-1} (2^n - 2^i) */
    seq_t gl = 1;
    seq_t two_n = pow2(n);          /* 2^n as seq_t */

    for (idx_t i = 0; i < n; ++i) {
        seq_t term = two_n - pow2(i);
        gl *= term;
    }

    /* n! * 2^{n*(n-1)/2} */
    seq_t fact = factorial(n);
    idx_t exp = n * (n - 1) / 2;    /* exponent fits in idx_t for n <= 10000 */
    seq_t two_exp = pow2(exp);
    seq_t correction = fact * two_exp;

    return gl - correction;
}
