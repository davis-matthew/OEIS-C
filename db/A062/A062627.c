/*
 * A062627  a(n) = mu(n) * Catalan(n).
 * https://oeis.org/A062627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = mu(n) * Catalan(n)
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 16, i64 n <= 36, i128 n <= 68,
 *            big any n
 * Helpers:   catalan, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* OEIS defines a(0)=1 (by convention mu(0)=1 and Catalan(0)=1). */
        return 1;
    }
    seq_t mu = mobius(n);      /* Möbius function μ(n) */
    seq_t cat = catalan(n);    /* Catalan number C_n = binomial(2n,n)/(n+1) */
    return mu * cat;
}
