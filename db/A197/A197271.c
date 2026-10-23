/*
 * A197271  a(n) = (10 / ((3*n+1)*(3*n+2))) * binomial(4*n, n).
 * https://oeis.org/A197271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 13, u64 n <= 23, u128 n <= 43,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A197271
   a(n) = 10/((3*n+1)*(3*n+2)) * binomial(4*n, n)
   Bounded recursive computation via the D‑finite recurrence:
   a(0) = 5
   a(n) = (8*(4*n-3)*(2*n-1)*(4*n-1) / (3*n*(3*n+1)*(3*n+2))) * a(n-1)   (n >= 1)
*/

#include "../utils/gcd.h"   /* provides seq_t gcd(seq_t a, seq_t b); */

seq_t a(idx_t n)
{
    /* base value a(0) = 5 */
    if (n == (idx_t)0)
        return (seq_t)5;

    seq_t res = (seq_t)5;          /* current a(k-1) during the loop */

    for (idx_t k = (idx_t)1; k <= n; ++k) {
        /* numerator N = 8*(4*k-3)*(2*k-1)*(4*k-1) */
        seq_t N = (seq_t)8;
        N *= ( (seq_t)4 * k - (seq_t)3 );
        N *= ( (seq_t)2 * k - (seq_t)1 );
        N *= ( (seq_t)4 * k - (seq_t)1 );

        /* denominator D = 3*k*(3*k+1)*(3*k+2) */
        seq_t D = (seq_t)3 * k;
        D *= ( (seq_t)3 * k + (seq_t)1 );
        D *= ( (seq_t)3 * k + (seq_t)2 );

        /* Reduce res with D */
        seq_t g1 = gcd(res, D);
        res /= g1;
        D   /= g1;

        /* Reduce N with remaining D */
        seq_t g2 = gcd(N, D);
        N     /= g2;
        D     /= g2;         /* D should now be 1 for integer result */

        /* Multiply res by remaining part of N */
        res *= N;
    }

    return res;
}
