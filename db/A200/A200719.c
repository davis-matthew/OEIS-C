/*
 * A200719  G.f. satisfies A(x) = (1 + x*A(x)^2) * (1 + x^2*A(x)^5).
 * https://oeis.org/A200719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/2⌋} (binom(2n+k+1, k) * binom(2n+k+1, n-2k)) /
 *            (2n+k+1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 48,
 *            big any n
 * Helpers:   binom, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/binom.h"    /* provides seq_t binom(idx_t n, idx_t k) */
#include "../utils/gcd.h"      /* provides seq_t gcd(seq_t a, seq_t b) */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 2; ++k)
    {
        /* N = 2*n + k + 1 (fits in idx_t for the required n range) */
        idx_t N = 2 * n + k + 1;

        seq_t b1 = binom(N, k);              /* C(N, k)              */
        seq_t b2 = binom(N, n - 2 * k);      /* C(N, n-2k)          */
        seq_t denom = (seq_t)N;              /* (2n+k+1)             */

        /* Cancel common factors with the denominator */
        seq_t g1 = gcd(b1, denom);
        b1 /= g1;
        denom /= g1;

        seq_t g2 = gcd(b2, denom);
        b2 /= g2;
        denom /= g2;                         /* denom should be 1 */

        seq_t term = b1 * b2;
        if (denom != 1) term /= denom;       /* safety, should not happen */

        sum += term;
    }
    return sum;
}
