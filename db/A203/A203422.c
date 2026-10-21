/*
 * A203422  Reciprocal of Vandermonde determinant of (1/2,1/3,...,1/(n+1)).
 * https://oeis.org/A203422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n+1)^(n-1) * Product_{i=2..n} (-i)^(i-1)
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 5, i64 n <= 6, i128 n <= 9,
 *            big any n
 * Helpers:   pow_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_val.h"

seq_t a(idx_t n) {
    /* No value is defined for n < 1 in the OEIS sequence; return 0 safely. */
    if (n < 1) return 0;

    /* Determine the overall sign: (-1)^{binomial(n,2)}.
       binomial(n,2) is odd iff n mod 4 is 2 or 3. */
    seq_t sign = 1;
    idx_t mod4 = n & 3;
    if (mod4 == 2 || mod4 == 3) {
        sign = -1;
    }

    /* Compute the magnitude */
    seq_t res = 1;
    for (idx_t i = 2; i <= n; ++i) {
        res *= pow_val((seq_t)i, i - 1);
    }
    res *= pow_val((seq_t)(n + 1), n - 1);

    /* Apply the sign */
    if (sign < 0) {
        res = -res;
    }
    return res;
}
