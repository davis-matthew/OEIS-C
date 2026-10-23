/*
 * A144650  Triangle read by rows where T(m,n) = 2m*n + m + n + 1.
 * https://oeis.org/A144650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 2*m*j + m + j + 1 where m = smallest integer with
 *            m*(m+1)/2 ≥ n, j = n - (m-1)*m/2 (i.e. triangle read by rows
 *            with T(m,n)=2*m*n+m+n+1)
 * Range:     u8 n <= 65, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* offset: sequence starts at n = 1 */

    /* Determine the row m using the triangular number formula.
       We use long double to approximate sqrt(8*n+1) and adjust. */
    unsigned long long m;
    {
        __int128 tmp = (__int128)8 * (unsigned __int128)n + 1;
        long double val = (long double)tmp;
        long double sqrtv = sqrtl(val);
        m = (unsigned long long)((sqrtv - 1.0L) / 2.0L);
    }

    /* Adjust m to satisfy m*(m+1)/2 ≥ n */
    while ((unsigned __int128)m * (m + 1) / 2 < n) m++;
    while (m > 0 && ((unsigned __int128)(m - 1) * m / 2) >= n) m--;

    /* Column index within the row */
    unsigned long long prev_tri = (unsigned __int128)(m - 1) * m / 2;
    unsigned long long j = n - prev_tri;  /* j is 1‑based */

    /* Compute the triangle entry T(m,j) */
    __int128 result = (__int128)2 * m * j + m + j + 1;
    return (seq_t)result;
}
