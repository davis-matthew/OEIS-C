/*
 * A132341  Main diagonal of A132339.
 * https://oeis.org/A132341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n, n) * binomial(4*n-2, 2*n-1) /
 *            ((2*n)*(2*n-1)), with a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* multiplicative binomial with cancelation – result fits in seq_t if the
   true value does (see algorithm in comment). */
static seq_t binom_cancel(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = (seq_t)(n - k + i);
        seq_t den = (seq_t)i;

        /* cancel common factors between numerator and denominator */
        seq_t g = gcd(num, den);
        if (g > 1) {
            num /= g;
            den /= g;
        }

        /* cancel remaining denominator with current result */
        g = gcd(res, den);
        if (g > 1) {
            res /= g;
            den /= g;
        }

        /* multiply; den must now be 1 */
        res *= num;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* first binomial: C(2n, n) */
    idx_t two_n = n * 2ULL;
    seq_t c1 = binom_cancel(two_n, n);

    /* second binomial: C(4n-2, 2n-1) */
    idx_t four_n_minus2 = (n << 2) - 2ULL;
    idx_t two_n_minus1  = two_n - 1ULL;
    seq_t c2 = binom_cancel(four_n_minus2, two_n_minus1);

    /* denominator factors */
    seq_t denom1 = (seq_t)two_n;          /* 2*n      */
    seq_t denom2 = (seq_t)two_n_minus1;   /* 2*n-1    */

    /* cancel denom1 with c1, c2, and result as needed */
    seq_t g = gcd(c1, denom1);
    if (g > 1) {
        c1   /= g;
        denom1 /= g;
    }
    g = gcd(c2, denom1);
    if (g > 1) {
        c2   /= g;
        denom1 /= g;
    }
    g = gcd((seq_t)1, denom1);   /* result is initially 1 */
    if (g > 1) {
        denom1 /= g;
    }

    /* cancel denom2 with c1, c2, and result */
    g = gcd(c1, denom2);
    if (g > 1) {
        c1   /= g;
        denom2 /= g;
    }
    g = gcd(c2, denom2);
    if (g > 1) {
        c2   /= g;
        denom2 /= g;
    }
    g = gcd((seq_t)1, denom2);   /* result is still 1 */
    if (g > 1) {
        denom2 /= g;
    }

    /* At this point denom1 and denom2 should be 1.
       Multiply the remaining factors. */
    seq_t result = 1;
    result *= c1;
    result *= c2;

    return result;
}
