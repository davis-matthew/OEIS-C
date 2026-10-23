/*
 * A138333  C(n+9, 9)*(n+5)*(-1)^(n+1)*256/5.
 * https://oeis.org/A138333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = C(n+9,9)*(n+5)*(-1)^(n+1)*256/5
 * Range:     i8 none, i16 n <= 1, i32 n <= 12, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Static helper: compute binomial(n, k) with n>=k, k small */
static seq_t binom_small(idx_t n, idx_t k)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * ((seq_t)(n - k + i));
        res = res / ((seq_t)i);
    }
    return res;
}

seq_t a(idx_t n)
{
    /* binomial coefficient C(n+9,9) */
    seq_t bin = binom_small(n + 9, 9);

    /* multiply by (n+5) */
    seq_t mult = bin * (((seq_t)n) + 5);

    /* multiply by 256 */
    seq_t scaled = mult * 256;

    /* divide by 5 (exact) */
    scaled = scaled / 5;

    /* apply sign: (-1)^(n+1) is negative when n is even */
    if ((n & 1) == 0) {
        scaled = -scaled;
    }

    return scaled;
}
