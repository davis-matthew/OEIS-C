/*
 * A132464  Let df(n,k) = Product_{i=0..k-1} (n-i) be the descending factorial
 *          and let P(m,n) = df(n-1,m-1)^2*(2*n-m)/((m-1)!*m!). Sequence gives
 *          P(6,n).
 * https://oeis.org/A132464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-5)^2*(n-4)^2*(n-3)^2*(n-2)^2*(n-1)^2*(2*n-6)/86400
 *            with offset 1
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 22, u64 n <= 151, u128 n <= 8403,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Euclidean algorithm for seq_t */
static inline seq_t _gcd(seq_t a, seq_t b)
{
    while (b) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n <= 5) return 0;

    seq_t nn = (seq_t)n;
    seq_t term[6];
    term[0] = (nn - 5) * (nn - 5);
    term[1] = (nn - 4) * (nn - 4);
    term[2] = (nn - 3) * (nn - 3);
    term[3] = (nn - 2) * (nn - 2);
    term[4] = (nn - 1) * (nn - 1);
    term[5] = (seq_t)(2 * nn - 6);

    seq_t denom = 86400u;   /* constant divisor */
    seq_t result = 1;

    for (int i = 0; i < 6; ++i) {
        /* Reduce current term against remaining denominator */
        seq_t g = _gcd(term[i], denom);
        if (g > 1) {
            term[i] /= g;
            denom /= g;
        }

        /* Multiply into result */
        result *= term[i];

        /* Reduce result against remaining denominator */
        g = _gcd(result, denom);
        if (g > 1) {
            result /= g;
            denom /= g;
        }
    }

    /* Any remaining denominator factors must divide the product */
    if (denom > 1) {
        result /= denom;
    }

    return result;
}
