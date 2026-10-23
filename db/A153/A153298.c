/*
 * A153298  G.f.: A(x) = F(x*G(x)^3)^2 = F(G(x)-1)^2 where F(x) = G(x/F(x)) =
 *          1 + x*F(x)^2 is the g.f. of A000108 (Catalan) and G(x) = F(x*G(x))
 *          = 1 + x*G(x)^3 is the g.f. of A001764.
 * https://oeis.org/A153298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0..n} C(2k+2,k)/(k+1) * C(3n,n-k) * k/n (a(0)=1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the offset 0 */
    if (n == 0) return (seq_t)1;

    /* Allocate arrays for the needed binomial coefficients */
    idx_t len = n + 1;
    seq_t *b3 = (seq_t *)malloc(sizeof(seq_t) * len); /* C(3n, m), 0<=m<=n */
    seq_t *b2 = (seq_t *)malloc(sizeof(seq_t) * len); /* C(2k+2, k), 0<=k<=n */

    if (!b3 || !b2) {               /* Allocation failure */
        free(b3);
        free(b2);
        return (seq_t)0;
    }

    /* Precompute C(3n, m) for m = 0..n */
    b3[0] = (seq_t)1;                /* C(3n,0) = 1 */
    for (idx_t m = 1; m <= n; ++m) {
        /* C(3n,m) = C(3n,m-1) * (3n-(m-1)) / m */
        seq_t num = b3[m - 1] * (seq_t)(3 * n - (m - 1));
        seq_t den = (seq_t)m;
        b3[m] = num / den;          /* exact division */
    }

    /* Precompute C(2k+2, k) for k = 0..n */
    b2[0] = (seq_t)1;                /* C(2,0) = 1 */
    for (idx_t k = 1; k <= n; ++k) {
        /* C(2k+2,k) = C(2(k-1)+2,(k-1)) * (2k+2)(2k+1) / (k(k+2)) */
        seq_t num = b2[k - 1] * (seq_t)(2 * k + 2) * (seq_t)(2 * k + 1);
        seq_t den = (seq_t)k * (seq_t)(k + 2);
        b2[k] = num / den;          /* exact division */
    }

    /* Accumulate the sum over k = 1..n */
    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t part1 = b2[k] / (seq_t)(k + 1);          /* C(2k+2,k)/(k+1) */
        seq_t part2 = b3[n - k];                      /* C(3n,n-k) */
        seq_t prod   = part1 * part2 * (seq_t)k;       /* *k */
        sum += prod / (seq_t)n;                       /* divide by n */
    }

    /* Clean up */
    free(b3);
    free(b2);

    return sum;
}
