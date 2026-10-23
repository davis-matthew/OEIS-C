/*
 * A194334  Triangular array: g(n,k)=number of fractional parts (i*r) in
 *          interval [(k-1)/n, k/n], for 1<=i<=2n, 1<=k<=n, r=2-phi, where
 *          phi=(1+sqrt(5))/2, the golden ratio.
 * https://oeis.org/A194334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = g(N,K) where (N,K) is the unique pair with * n =
 *            N*(N-1)/2 + K, 1 <= K <= N, and * g(N,K) = # { i : 1 <= i <= 2N,
 *            frac(i*r) in [(K-1)/N , K/N) }, * r = 2 - phi = (3 - sqrt(5)) /
 *            2. * The function computes the count by brute force; 2N <= 2*141
 *            < 300 * for n <= 10000, so the work per call is...
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Find N such that N*(N-1)/2 < n <= N*(N+1)/2 */
    idx_t N = 1;
    /* Quadratic formula gives approximate N */
    double nd = (double)n;
    N = (idx_t)((sqrt(1.0 + 8.0*nd) - 1.0) / 2.0) + 1;
    while ((N*(N-1))/2 >= n) N--;
    while ((N*(N+1))/2 < n)   N++;

    idx_t prev_count = (N-1)*N/2;
    idx_t K = n - prev_count;   /* 1 <= K <= N */

    const double r = 0.381966011250105150;  /* 2 - phi */
    const double eps = 1e-12;              /* safety margin */

    seq_t cnt = 0;
    for (idx_t i = 1; i <= 2*N; ++i) {
        double frac = i * r - floor(i * r);
        double low  = (double)(K-1) / (double)N;
        double high = (double)K      / (double)N;
        if (frac + eps >= low && frac < high)
            ++cnt;
    }
    return cnt;
}
