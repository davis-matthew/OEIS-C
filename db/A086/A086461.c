/*
 * A086461  Symmetric version of square array A086460, read by antidiagonals.
 * https://oeis.org/A086461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = 1 when i==0 or j==0, otherwise a(n)=i*j, * where (i,j)
 *            are the coordinates on the symmetric array A(n,k)= * 1 if n==0
 *            or k==0, otherwise n*k, read by antidiagonals: * let s =
 *            floor((sqrt(8*n+1)-1)/2); let t = n - s*(s+1)/2; * then (i,j) =
 *            (t, s-t)
 * Range:     u8 n <= 543, u16 n <= 1325, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2 */
    idx_t s = (idx_t)(sqrt(2.0L * (long double)n) + 2.0L);
    /* Correct possible overestimation */
    while ((unsigned __int128)s * (s + 1) / 2 > n) --s;
    while ((unsigned __int128)(s + 1) * (s + 2) / 2 <= n) ++s;

    idx_t tri = (idx_t)((unsigned __int128)s * (s + 1) / 2);
    idx_t t = n - tri;               /* offset on the current diagonal */
    idx_t i = t;
    idx_t j = s - t;

    if (i == 0 || j == 0)
        return (seq_t)1;

    return (seq_t)i * (seq_t)j;
}
