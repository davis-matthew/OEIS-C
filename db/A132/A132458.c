/*
 * A132458  Let df(n,k) = Product_{i=0..k-1} (n-i) be the descending factorial
 *          and let P(m,n) = df(n-1,m-1)^2*(2*n-m)/((m-1)!*m!). Sequence gives
 *          P(4,n).
 * https://oeis.org/A132458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-3)^2 * (n-2)^3 * (n-1)^2 / 72 (for n ≥ 1)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 45, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (a(1) = a(2) = a(3) = 0 by convention) */

#include <stdint.h>

/* Compute gcd of two seq_t values */
static seq_t _gcd_seq_t(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n <= 3) return 0;

    /* Convert to seq_t once */
    seq_t A = (seq_t)(n - 3);
    seq_t B = (seq_t)(n - 2);
    seq_t C = (seq_t)(n - 1);

    /* List of factors: A*A, B*B*B, C*C  (7 factors total) */
    seq_t factors[7];
    factors[0] = A;
    factors[1] = A;
    factors[2] = B;
    factors[3] = B;
    factors[4] = B;
    factors[5] = C;
    factors[6] = C;

    seq_t denom = (seq_t)72;
    seq_t result = 1;

    /* Reduce denominators early to keep intermediate values small */
    for (int i = 0; i < 7; ++i) {
        seq_t g = _gcd_seq_t(factors[i], denom);
        if (g > 1) {
            factors[i] /= g;
            denom /= g;
        }
        result *= factors[i];
    }

    /* Any remaining denominator (should be 1) */
    if (denom > 1)
        result *= denom;

    return result;
}
