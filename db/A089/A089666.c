/*
 * A089666  a(n) = S2(n,3), where S2(n, t) = Sum_{k=0..n} k^t *(Sum_{j=0..k}
 *          binomial(n,j))^2.
 * https://oeis.org/A089666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(15*n^3 + 30*n^2 + 21*n - 2)*4^(n-3) -
 *            (n-1)^2*n^2*(n+1)*binomial(2*n, n)/(8*(2*n-1))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 53,
 *            big any n
 * Helpers:   big_pow, binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Handle the few small indices that would involve negative powers of 4 */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)137;

    /* ----- First term: n * (15*n^3 + 30*n^2 + 21*n - 2) * 4^(n-3) ----- */
    seq_t N   = (seq_t)n;
    seq_t poly = ((seq_t)15 * N * N * N)   /* 15*n^3  */
               + ((seq_t)30 * N * N)       /* 30*n^2  */
               + ((seq_t)21 * N)           /* 21*n    */
               - (seq_t)2;                 /* -2      */

    /* Compute n * poly */
    seq_t term1 = N * poly;

    /* Multiply by 4^(n-3) iteratively to avoid overflow for the uint64_t build */
    for (idx_t i = 0; i < n - 3; ++i)
        term1 *= (seq_t)4;

    /* ----- Second term: (n-1)^2*n^2*(n+1)*binomial(2*n,n)/(8*(2*n-1)) ----- */
    seq_t denom = (seq_t)8 * (seq_t)(2 * n - 1);
    seq_t bin   = binomial((idx_t)(2 * n), (idx_t)n);

    /* Reduce binomial value against the denominator first */
    seq_t g = gcd(bin, denom);
    bin   /= g;
    denom /= g;

    seq_t term2_num = (seq_t)1;
    /* Helper macro to reduce a factor against the current denominator and
       multiply it into the term2 numerator */
#define REDUCE_AND_MULTIPLY(factor)                                                \
    do {                                                                            \
        seq_t g2 = gcd(factor, denom);                                               \
        factor /= g2;                                                               \
        denom  /= g2;                                                               \
        term2_num *= factor;                                                        \
    } while (0)

    seq_t f1 = (seq_t)(n - 1);
    seq_t f2 = (seq_t)(n - 1);
    seq_t f3 = (seq_t)n;
    seq_t f4 = (seq_t)n;
    seq_t f5 = (seq_t)(n + 1);

    REDUCE_AND_MULTIPLY(f1);
    REDUCE_AND_MULTIPLY(f2);
    REDUCE_AND_MULTIPLY(f3);
    REDUCE_AND_MULTIPLY(f4);
    REDUCE_AND_MULTIPLY(f5);
    REDUCE_AND_MULTIPLY(bin);

    /* If any part of the denominator remains, divide it now */
    if (denom != (seq_t)1)
        term2_num /= denom;

    seq_t term2 = term2_num;

    /* Final value */
    return term1 - term2;
}
