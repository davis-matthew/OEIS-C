/*
 * A223173  Poly-Cauchy numbers c_3^(-n).
 * https://oeis.org/A223173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2^(n+1) - 3^(n+1) + 4^n
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 15, i64 n <= 31, i128 n <= 63,
 *            big any n
 * Helpers:   pow2, pow3, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    seq_t two   = pow2(n + 1);  /* 2^(n+1) */
    seq_t three = pow3(n + 1);  /* 3^(n+1) */
    seq_t four  = pow4(n);      /* 4^n      */

    return two - three + four;
}
