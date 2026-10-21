/*
 * A069736  Total number of Eulerian circuits in labeled multigraphs with n
 *          edges.
 * https://oeis.org/A069736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((2*n)!/(2^n*n!) * (3^(n+1)-1))/(2*(n+1))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/gcd.h"

/* Return the n-th term of A069736 (total number of Eulerian circuits in
   labeled multigraphs with n edges).  */
seq_t a(idx_t n)
{
    /* (2n-1)!! = (2n)!/(2^n n!) */
    seq_t A = 1;
    for (idx_t i = 1; i <= n; ++i) {
        A *= ((seq_t)2 * (seq_t)i - 1);
    }

    /* 3^(n+1) - 1 */
    seq_t pow3 = big_pow(3, (seq_t)(n + 1));
    seq_t B = pow3 - 1;

    /* denominator = 2*(n+1) */
    seq_t denom = (seq_t)2 * ((seq_t)n + 1);

    /* Reduce A with denom */
    seq_t g = gcd(A, denom);
    if (g > 1) {
        A /= g;
        denom /= g;
    }

    /* Reduce B with remaining denom */
    g = gcd(B, denom);
    if (g > 1) {
        B /= g;
        denom /= g;
    }

    /* denom should now be 1; the product is the desired term */
    return A * B;
}
