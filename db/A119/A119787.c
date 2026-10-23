/*
 * A119787  Numerator of the product of n and the n-th alternating harmonic
 *          number, Sum_{k=1..n} (-1)^(k+1)/k.
 * https://oeis.org/A119787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator( n * Σ_{k=1..n} (-1)^{k+1} / k )
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 22, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The value is the numerator of n times the n‑th alternating harmonic number,
   reduced to lowest terms. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* safety for out‑of‑range calls */

    seq_t N = 1;                          /* numerator of H_1  */
    seq_t D = 1;                          /* denominator of H_1 */

    for (idx_t k = 2; k <= n; ++k) {
        seq_t kseq = (seq_t)k;
        bool add = ((k & 1U) == 1U);      /* odd k: add the 1/k term,          */
                                          /* even k: subtract the 1/k term      */

        /* common denominator: lcm(D, k) */
        seq_t g1 = gcd(D, kseq);
        seq_t lcm = (D / g1) * kseq;

        /* new numerator before reduction */
        seq_t multN    = N * (lcm / D);    /* N * (lcm/D)   */
        seq_t multTerm = (lcm / kseq);     /* 1 * (lcm/k)   */
        seq_t newN = add ? (multN + multTerm) : (multN - multTerm);

        seq_t newD = lcm;

        /* reduce the fraction */
        seq_t g2 = gcd(newN, newD);
        N = newN / g2;
        D = newD / g2;
    }

    /* a(n) = (n * N) / gcd(n*N, D)  */
    seq_t an = ((seq_t)n) * N;
    seq_t g = gcd(an, D);
    return an / g;
}
