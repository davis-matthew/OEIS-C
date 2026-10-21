/*
 * A219069  Triangle read by rows: T(n,k) = n^4 + (n*k)^2 + k^4, 1 <= k <= n.
 * https://oeis.org/A219069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, look
 * Formula:   a(n) = r^4 + (r*k)^2 + k^4, where r is the smallest * integer
 *            such that r*(r+1)/2 >= n, and k = n - (r-1)*r/2
 * Range:     u8 n <= 6, u16 n <= 89, u32 n <= 7260, u64 n <= 7260,
 *            u128 n <= 7260, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;             /* Offset is 1; n==0 is undefined */

    /* Find r by solving r*(r+1)/2 >= n */
    long double val = sqrtl((long double)(8.0L * (long double)n + 1.0L));
    idx_t r = (idx_t)((val - 1.0L) / 2.0L);   /* floor approximation */

    /* Adjust to the exact smallest r */
    while (( (__int128)r * (r + 1) ) / 2 < n) ++r;
    while (r > 1 && ( (__int128)(r - 1) * r ) / 2 >= n) --r;

    /* Compute k */
    idx_t k = n - ( (__int128)(r - 1) * r ) / 2;

    /* Compute T(r,k) = r^4 + (r*k)^2 + k^4 in seq_t arithmetic */
    seq_t sr = (seq_t)r;
    seq_t sk = (seq_t)k;
    seq_t r2  = sr * sr;          /* r^2 */
    seq_t r4  = r2 * r2;          /* r^4 */
    seq_t k2  = sk * sk;          /* k^2 */
    seq_t k4  = k2 * k2;          /* k^4 */
    seq_t rjk = sr * sk;          /* r*k */
    seq_t rjk2= rjk * rjk;        /* (r*k)^2 */

    return r4 + rjk2 + k4;
}
