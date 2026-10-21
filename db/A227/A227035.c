/*
 * A227035  a(n) = Sum_{k=0..floor(n/4)}
 *          binomial(n,4*k)*binomial(5*k,k)/(4*k+1).
 * https://oeis.org/A227035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/4)}
 *            binomial(n,4k)*binomial(5k,k)/(4k+1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 26, u64 n <= 48, u128 n <= 91,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 4U;                     /* floor(n/4) */
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t fourk = 4U * k;
        seq_t term1 = binomial(n, fourk);    /* binomial(n,4k) */
        seq_t term2 = binomial(5U * k, k);   /* binomial(5k,k) */
        seq_t C = 4U * k + 1U;                /* denominator (4k+1) */

        /* Reduce factors by gcd to keep numbers small. */
        seq_t g1 = gcd(term1, C);
        if (g1 > 1U) { term1 /= g1; C /= g1; }

        seq_t g2 = gcd(term2, C);
        if (g2 > 1U) { term2 /= g2; C /= g2; }

        /* After reductions the remaining denominator is 1.  If it
           is still >1 (possible for very small types), perform the
           division after the multiplication; the division is exact. */
        if (C > 1U) {
            term1 *= term2;
            term1 /= C;
        } else {
            term1 *= term2;
        }
        sum += term1;
    }
    return sum;
}
