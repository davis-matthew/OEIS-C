/*
 * A078488  First differences of coefficients of g.f. (1-x)^24.
 * https://oeis.org/A078488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, less, fini, full
 * Formula:   a(n) = (-1)^n * binomial(25, n) (offset 1)
 * Range:     i8 n <= 1, i16 n <= 4, i32 n <= 24, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* For n > 25 the binomial coefficient is zero (outside the binomial
       range).  The sequence is defined only for n >= 1, but we return 0
       for all n > 25 to be safe. */
    if (n == 0 || n > 25) {
        return (seq_t)0;
    }

    /* Compute binomial(25, n) multiplicatively.
       Use the smaller of n and 25-n to reduce the work. */
    idx_t k = n;
    if (k > 25 - k) {
        k = 25 - k;
    }

    seq_t result = (seq_t)1;
    idx_t start = 25 - k + 1;   /* start value for the numerator */

    for (idx_t i = 0; i < k; ++i) {
        /* Multiply by (start + i) then divide by (i+1); the division
           is exact at each step because we are computing a binomial
           coefficient. */
        result = result * (seq_t)(start + i) / (seq_t)(i + 1);
    }

    /* Apply the sign (-1)^n.  For odd n the result is negated. */
    if ((n & 1ULL) == 1) {
        result = -result;
    }

    return result;
}
