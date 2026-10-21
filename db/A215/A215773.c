/*
 * A215773  Number of undirected labeled graphs on n+3 nodes with exactly n
 *          cycle graphs as connected components.
 * https://oeis.org/A215773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)*(n^2+n+4)/48
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 75, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Handle n = 0 directly (formula gives 0) */
    if (n == 0)
        return 0;

    /* All computations are performed in seq_t to preserve precision. */
    seq_t N = (seq_t)n;
    seq_t prod = (seq_t)1;
    seq_t denom = (seq_t)48;

    /* Sequentially process each factor, cancelling gcd with the
       remaining denominator to keep intermediate values small. */

    /* Factor n */
    seq_t f = N;
    seq_t g = gcd(f, denom);
    f /= g;          /* cancel common factor */
    denom /= g;
    prod *= f;

    /* Factor n+1 */
    f = N + 1;
    g = gcd(f, denom);
    f /= g;
    denom /= g;
    prod *= f;

    /* Factor n+2 */
    f = N + 2;
    g = gcd(f, denom);
    f /= g;
    denom /= g;
    prod *= f;

    /* Factor n+3 */
    f = N + 3;
    g = gcd(f, denom);
    f /= g;
    denom /= g;
    prod *= f;

    /* Factor n^2 + n + 4 */
    seq_t NN = N;
    f = NN * NN + NN + 4;
    g = gcd(f, denom);
    f /= g;
    denom /= g;      /* denom should now be 1 */
    prod *= f;

    /* The remaining denom must be 1.  If it is not 1 the division
       would not be exact, but the formula guarantees exactness. */
    return prod;
}
