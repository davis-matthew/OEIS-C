/*
 * A138896  Ratio of (2n-1)! to number of zeros in Sylvester matrix of
 *          polynomial of n degree with all nonzero coefficients.
 * https://oeis.org/A138896
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n-1)! / (2*(n-1)^2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* offset: the sequence is defined for n >= 2 */
    if (n < 2) return 0;  /* undefined behavior for n < 2 */

    /* denominator d = 2*(n-1)^2 */
    idx_t d = (idx_t)2 * (n - 1) * (n - 1);

    seq_t result = 1;

    for (idx_t i = 1; i <= 2 * n - 1; ++i) {
        idx_t term = i;
        idx_t g = gcd_idx(term, d);
        if (g > 1) {
            term /= g;
            d /= g;
        }
        result *= (seq_t)term;
    }

    /* If any part of the denominator remained, divide it now */
    if (d > 1) {
        result /= (seq_t)d;
    }

    return result;
}
