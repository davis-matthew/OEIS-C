/*
 * A117251  Column 0 of triangle A117250.
 * https://oeis.org/A117251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^(n-1) * C(2*n,n)/(2*n-1) * 2^((n-1)*(n-2)/2-1)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 11, i128 n <= 15,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* binomial coefficient C(2*n, n) */
    seq_t bin = binomial(2*n, n);

    /* divide by (2*n - 1) */
    seq_t val = bin / (seq_t)(2*n - 1);

    /* exponent for power of 2 */
    int64_t n1 = (int64_t)n - 1;
    int64_t n2 = (int64_t)n - 2;
    int64_t exp = (n1 * n2) / 2 - 1;

    if (exp < 0) {
        /* divide by 2^(-exp) */
        seq_t div = pow2((idx_t)(-exp));
        val = val / div;
    } else if (exp > 0) {
        /* multiply by 2^exp */
        seq_t mul = pow2((idx_t)exp);
        val = val * mul;
    }

    /* apply sign (-1)^(n-1) */
    if ((n - 1) & 1) val = -val;
    return val;
}
