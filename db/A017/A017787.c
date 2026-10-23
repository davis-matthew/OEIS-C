/*
 * A017787  Binomial coefficients C(71,n).
 * https://oeis.org/A017787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial(71,n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 26, u128 n <= 71,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute C(71,n) multiplicatively with GCD reduction to keep intermediate
 *  values small.  For n>71 the binomial coefficient is defined to be 0. */
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n > 71)
        return (seq_t)0;
    /* use symmetry to minimize the number of iterations */
    idx_t k = n;
    if (k > 71 - k)
        k = 71 - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = (seq_t)(71 - k + i);   /* numerator */
        seq_t d1 = gcd(res, (seq_t)i);     /* reduce res with i */
        res /= d1;
        seq_t i_red = (seq_t)i / d1;       /* remaining divisor */
        seq_t d2 = gcd(num, i_red);        /* reduce numerator with remaining divisor */
        num /= d2;
        res *= num;                        /* multiply after reduction */
    }
    return res;
}
