/*
 * A160847  Number of lines through at least 2 points of a 7 X n grid of
 *          points.
 * https://oeis.org/A160847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/2 * (f(7,n,1) - f(7,n,2)), where f(m,n,k) =
 *            Σ_{-n<k*x<n, -m<k*y<m, gcd(|x|,|y|)=1} (n-|k*x|)*(m-|k*y|)
 * Range:     u8 n <= 2, u16 n <= 51, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation follows the definition directly.
   It runs in O(n) time for each call (with m=7) and is well
   within the required performance limits for n up to about 10 000. */

#include <stdint.h>

/* Euclidean GCD for positive long long values */
static long long llgcd(long long a, long long b)
{
    while (b != 0) {
        long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Compute f(m,n,k) as defined above.  m is fixed at 7. */
static seq_t compute_f(idx_t n, int k)
{
    /* n is non‑negative; for n==0 there are no points.  */
    if (n == 0)
        return 0;

    seq_t sum = 0;
    /* maximum absolute values of x and y that satisfy -n < k*x < n etc. */
    long long xmax = (long long)(n - 1) / k;
    long long ymax = (long long)(7 - 1) / k;   /* m = 7 */

    for (long long x = -xmax; x <= xmax; ++x) {
        long long absx = (x < 0) ? -x : x;
        for (long long y = -ymax; y <= ymax; ++y) {
            long long absy = (y < 0) ? -y : y;
            if (x == 0 && y == 0)
                continue;                 /* skip zero vector */
            if (llgcd(absx, absy) != 1)
                continue;                 /* only primitive vectors */

            /* compute |k*x| and |k*y|, guaranteed < n and < 7 */
            idx_t kx = (idx_t)k * (idx_t)absx;
            idx_t ky = (idx_t)k * (idx_t)absy;

            /* term = (n - |k*x|) * (7 - |k*y|) */
            seq_t term = ((seq_t)(n - kx)) * ((seq_t)(7 - ky));
            sum += term;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;      /* a(0) = 0 by definition */

    seq_t f1 = compute_f(n, 1);
    seq_t f2 = compute_f(n, 2);
    return (f1 - f2) / 2;
}
