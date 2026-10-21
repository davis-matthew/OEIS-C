/*
 * A215552  a(n) = binomial(8*n,n)*(6*n+1)/(7*n+1).
 * https://oeis.org/A215552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(8*n, n)*(6*n+1)/(7*n+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>       /* for uint64_t, if needed */
#include "../utils/gcd.h" /* for gcd(seq_t, seq_t) */

seq_t a(idx_t n)
{
    /* Handle the trivial case first */
    if (n == 0)
        return (seq_t)1;

    seq_t nseq = (seq_t)n;
    seq_t bin  = (seq_t)1;

    /* Compute binomial(8*n, n) with cancellation to avoid overflow */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t num = (seq_t)(8 * nseq - k + 1);
        seq_t den = (seq_t)k;

        /* cancel common factors between the current numerator and denominator */
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        /* cancel any remaining denominator factors with the running binomial */
        g = gcd(bin, den);
        bin /= g;
        den /= g;

        /* den should now be 1; multiply the reduced numerator */
        bin = bin * num / den;   /* den == 1 so this is just bin *= num; */
    }

    /* Multiply by (6*n+1)/(7*n+1) with further cancellations */
    seq_t num = (seq_t)(6 * nseq + 1);
    seq_t den = (seq_t)(7 * nseq + 1);

    seq_t g = gcd(num, den);
    num /= g;
    den /= g;

    g = gcd(bin, den);
    bin /= g;
    den /= g;

    /* den should be 1; final multiplication */
    bin = bin * num / den;   /* again den == 1 */

    return bin;
}
