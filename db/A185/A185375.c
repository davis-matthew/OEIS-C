/*
 * A185375  a(n) = n*(n-1)*(2*n+1)*(2*n-1)*(2*n-3)*(10*n-17)/90.
 * https://oeis.org/A185375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n-1)*(2*n+1)*(2*n-1)*(2*n-3)*(10*n-17)/90
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 41, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A185375 for all n ≥ 0.  Uses only integer arithmetic. */

#include <stddef.h>

/* Euclidean gcd for seq_t */
static seq_t gcd_seq(seq_t a, seq_t b) {
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n) {
    /* Terms for n = 0 and 1 are 0. */
    if (n < 2) return 0;

    seq_t factors[6];
    seq_t nn = (seq_t)n;
    factors[0] = nn;                                 /* n */
    factors[1] = nn - 1;                             /* n-1 */
    factors[2] = (seq_t)(2*n + 1);                   /* 2n+1 */
    factors[3] = (seq_t)(2*n - 1);                   /* 2n-1 */
    factors[4] = (seq_t)(2*n - 3);                   /* 2n-3 */
    factors[5] = (seq_t)(10*n - 17);                 /* 10n-17 */

    seq_t denom = (seq_t)90;
    seq_t product = (seq_t)1;

    for (int i = 0; i < 6; ++i) {
        /* Cancel as much of the denominator as possible from this factor. */
        seq_t g = gcd_seq(factors[i], denom);
        if (g > (seq_t)1) {
            factors[i] /= g;
            denom /= g;
        }
        product *= factors[i];

        /* After multiplication, reduce product with the remaining denominator
           to keep numbers smaller. */
        g = gcd_seq(product, denom);
        if (g > (seq_t)1) {
            product /= g;
            denom /= g;
        }
    }

    /* Any remaining denominator (might be 1) is divided last. */
    if (denom > (seq_t)1) {
        product /= denom;
    }

    return product;
}
