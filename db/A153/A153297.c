/*
 * A153297  G.f.: A(x) = F(x*G(x)^2)^2 where F(x) = G(x/F(x)) = 1 + x*F(x)^2
 *          is the g.f. of A000108 (Catalan) and G(x) = F(x*G(x)) = 1 +
 *          x*G(x)^3 is the g.f. of A001764.
 * https://oeis.org/A153297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A153297
   Formula: a(n) = Σ_{k=0..n} C(2k+2,k)/(k+1) * C(3n-k,n-k) * 2k/(3n-k)
   for n>0, with a(0)=1.
   Implements the sum using multiplicative recurrences to run in O(n)
   time per call. */

#include <stdint.h>

/* Compute binomial coefficient C(n,k) with multiplicative formula.
   Assumes 0 ≤ k ≤ n. */
static seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * ( (seq_t)(n - k + i) );
        res = res / (i);
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    idx_t three_n = 3 * n;

    /* Prefix product for B_k : C(3n-k, n-k) */
    seq_t B = binom_idx(three_n, n);          /* B_0 */

    /* Current value of A_k = C(2k+2,k)/(k+1) = Catalan(k+1) */
    seq_t A = 1;                               /* A_0 */

    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ++k) {
        /* Update A_k from A_{k-1}:
           A_k = A_{k-1} * (2*(2*(k-1)+3)) / (k+2) */
        seq_t numA = (seq_t)2 * ( (seq_t)(2 * (k - 1) + 3) );
        seq_t denA = k + 2;
        A = A * numA / denA;

        /* Update B_k from B_{k-1}:
           B_k = B_{k-1} * (n-(k-1)) / (3n-(k-1)) */
        seq_t numB = n - (k - 1);
        seq_t denB = three_n - (k - 1);
        B = B * numB / denB;

        /* term = A * B * 2k / (3n - k) */
        seq_t term = A * B;
        term *= (seq_t)(2 * k);
        term /= (three_n - k);

        sum += term;
    }

    return sum;
}
