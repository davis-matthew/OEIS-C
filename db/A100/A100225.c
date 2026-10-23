/*
 * A100225  G.f. A(x) satisfies: 3^n - 1 = Sum_{k=0..n} [x^k]A(x)^n and also
 *          satisfies: (3+z)^n - (1+z)^n + z^n = Sum_{k=0..n}
 *          [x^k](A(x)+z*x)^n for all z, where [x^k]A(x)^n denotes the
 *          coefficient of x^k in A(x)^n.
 * https://oeis.org/A100225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 15, i32 n <= 27, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   catalan, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (closed form):
   a(0)=1, a(1)=1, a(2)=2.
   For n≥3,
   a(2·m) = 2^m · (-1)^(m-1) · Catalan(m-1),
   a(2·m+1) = 0 for m≥1
   (Catalan(k) = binom(2k,k)/(k+1)) */
#include <stdint.h>
#include "../utils/catalan.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* handle initial values */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)2;

    /* odd n ≥ 3 → 0 */
    if (n & (idx_t)1) return (seq_t)0;

    /* even n ≥ 4 */
    idx_t m = n >> 1;                /* m = n/2 */
    seq_t val = pow2(m) * catalan(m - (idx_t)1);

    /* sign = (-1)^(m-1) : it is negative when m is even */
    if ((m & (idx_t)1) == 0) {
        val = -(seq_t)val;
    }
    return val;
}
