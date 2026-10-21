/*
 * A073644  Largest k such that the harmonic mean of phi(n), phi(n+1), ....,
 *          phi(n+x) is an integer for any x with 0<=x<=k.
 * https://oeis.org/A073644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   gcd, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A073644:  largest k such that (x+1)/sum(i=0,x,1/phi(n+i))
    is integer for 0 <= x <= k.  Computed by incrementally
    adding 1/phi and checking the condition. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/phi.h"   /* seq_t phi(idx_t m); */
#include "../utils/gcd.h"   /* seq_t gcd(seq_t a, seq_t b); */

seq_t a(idx_t n)
{
    seq_t N = 0;     /* numerator of Σ 1/phi */
    seq_t D = 1;     /* denominator of Σ 1/phi */

    for (idx_t x = 0; ; ++x) {
        seq_t phi_val = phi(n + x);          /* phi(n+x) */
        /* Update sum: N/D + 1/phi = (N*phi + D)/(D*phi) */
        seq_t newN = N * phi_val + D;
        seq_t newD = D * phi_val;
        seq_t g = gcd(newN, newD);
        N = newN / g;
        D = newD / g;

        /* Harmonic mean: (x+1)*D / N  */
        seq_t numerator = ((seq_t)(x + 1)) * D;
        if (numerator % N != 0) {
            /* Last valid k is x-1 */
            return (seq_t)(x - 1);
        }
        /* In practice a(n) <= 5, so the loop will terminate quickly. */
    }
}
