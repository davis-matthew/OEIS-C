/*
 * A219287  Number of n X 4 arrays of the minimum value of corresponding
 *          elements and their horizontal or vertical neighbors in a random,
 *          but sorted with lexicographically nondecreasing rows and columns,
 *          0..1 n X 4 array.
 * https://oeis.org/A219287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is a polynomial of degree 8 given by the generating
 *            function G(x) = x*(4 - 27x + 105x² - 241x³ + 342x⁴ - 249x⁵ + x⁶
 *            + 204x⁷ - 240x⁸ + 223x⁹ - 206x¹⁰ + 161x¹¹ - 76x¹² + 22x¹³ -
 *            5x¹⁴) / (1 - x)⁹ (OEIS A219287) Equivalently a(n) = Σ_{j=0..14}
 *            P_j * C(n+7-j, 8) where P_j are the coefficients of...
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 43, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdint.h>   /* for uint64_t */

/* maximum of 14 terms in the numerator */
static const int P[15] = {
    4,   -27,  105,  -241,   342,
   -249,   1,  204,  -240,   223,
   -206,  161,   -76,    22,   -5
};

/* compute binomial coefficient C(n,8) for n >= 8,
   using a multiplicative algorithm that keeps the result
   integer at every step.  n is non‑negative, n >= 8. */
static inline seq_t binom8(idx_t n)
{
    seq_t res = 1;
    /* n-8+i ranges from n-7 to n */
    for (idx_t i = 1; i <= 8; ++i) {
        res = res * (seq_t)(n - 8 + i);
        res /= i;                /* division is exact */
    }
    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n < 1, return 0 */

    seq_t pos = 0;   /* sum of positive terms */
    seq_t neg = 0;   /* sum of negative terms */

    for (idx_t j = 0; j < 15; ++j) {
        /* term contributes only if n > j, otherwise C(n+7-j,8)=0 */
        if (n <= j) continue;

        idx_t t = n + 7 - j;           /* the upper argument of C(·,8) */
        seq_t bin = binom8(t);         /* binomial term */
        int coeff = P[j];

        if (coeff >= 0)
            pos += bin * (seq_t)coeff;
        else
            neg += bin * (seq_t)(-coeff);
    }
    return pos - neg;
}
