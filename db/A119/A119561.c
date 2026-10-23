/*
 * A119561  Define F(n) = 2^(2^n)+1 = the n-th Fermat number, M(n) = 2^n-1 =
 *          the n-th Mersenne number. Then a(n) =
 *          F(n)+M(n)+1=2^(2^n)+1+2^n-1+1 = 2^(2^n)+2^n+1 = F(n)+2^n.
 * https://oeis.org/A119561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2^n) + 2^n + 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* compute 2^n */
    seq_t two_n = pow2(n);

    /* compute 2^(2^n).  The exponent 2^n fits in idx_t for n < 64. */
    idx_t exp = ((idx_t)1 << n);  /* 2^n */

    seq_t f = pow2(exp);          /* 2^(2^n) */

    /* result = 2^(2^n) + 2^n + 1 */
    return f + two_n + 1;
}
