/*
 * A055306  Number of labeled rooted trees with n nodes and 5 leaves.
 * https://oeis.org/A055306
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A055306
   Formula: a(n) = n! * (n-5)*(n-4)*(n-3)*(n-2)*(n-1)*(15*n^3 - 195*n^2 + 830*n - 1152) / 691200
   (valid for n ≥ 6) */

#include <stdint.h>

/* Euclidean GCD for values of type seq_t */
static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 6) {
        return 0;          /* undefined below the offset */
    }

    seq_t result = 1;                     /* running product */
    seq_t denom  = 691200;                 /* denominator to cancel */

    /* factorial part: 2 .. n */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t f = (seq_t)i;
        seq_t g = gcd_seq(f, denom);
        if (g != 1) {
            denom /= g;
            f /= g;
        }
        result *= f;
    }

    /* extra product: (n-5) .. (n-1) */
    for (idx_t i = n - 5; i <= n - 1; ++i) {
        seq_t f = (seq_t)i;
        seq_t g = gcd_seq(f, denom);
        if (g != 1) {
            denom /= g;
            f /= g;
        }
        result *= f;
    }

    /* polynomial factor Q = 15*n^3 - 195*n^2 + 830*n - 1152 */
    seq_t ns = (seq_t)n;
    seq_t n2 = ns * ns;
    seq_t n3 = n2 * ns;
    seq_t Q  = 15 * n3 - 195 * n2 + 830 * ns - 1152;

    seq_t g = gcd_seq(Q, denom);
    if (g != 1) {
        denom /= g;
        Q /= g;
    }
    result *= Q;

    /* any remaining denominator factor must divide the result */
    if (denom != 1) {
        result /= denom;
    }

    return result;
}
