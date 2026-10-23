/*
 * A101799  a(n)= det[A000522(i+j+1)], i,j=0...n, is the Hankel determinant of
 *          order n+1 of the arrangements numbers, s. A000522; a(n) = product(
 *          (p!)^2,p=0..n )*(n+1)!*LaguerreL(n+1,0,-1), n=0,1..., where
 *          LaguerreL(n,lambda,x) are generalized Laguerre polynomials;
 *          a(n)=A055209(n)*A002720(n+1);.
 * https://oeis.org/A101799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (∏_{p=0}^{n} (p!)^2) * (n+1)! * LaguerreL(n+1,0,-1)
 *            Equivalent integer formula: a(n) = Σ_{k=0}^{n+1} ((n+1)!)^2 *
 *            (∏_{p=0}^{n} p!)^2 / ((n+1-k)! * (k!)^2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* N = n+1, used for factorial (n+1)! */
    idx_t N = n + 1;

    /* Compute factorials iteratively */
    seq_t fact = 1;   /* current factorial (p!)          */
    seq_t F    = 1;   /* product of factorials ≤ n (∏ p!) */

    for (idx_t p = 0; p <= n; ++p) {
        F *= fact;               /* multiply by p!            */
        if (p < n) fact *= (p + 1); /* update to (p+1)!       */
    }
    /* Now fact == n! */
    seq_t fact_n  = fact;
    seq_t fact_N  = fact_n;       /* (n+1)! will be computed below */
    fact_N *= (n + 1);            /* multiply by (n+1) */

    /* term_0 = (n+1)! * F^2   (k = 0 in the sum) */
    seq_t term = fact_N;
    term *= F;
    term *= F;

    /* sum of terms in the series */
    seq_t sum = term;

    for (idx_t k = 0; k < N; ++k) {
        /* term_{k+1} = term_k * (N-k) / ((k+1)^2)  */
        term *= (seq_t)(N - k);

        seq_t denom = (seq_t)(k + 1);
        denom *= denom;          /* (k+1)^2 */
        term /= denom;

        sum += term;
    }

    return sum;
}
