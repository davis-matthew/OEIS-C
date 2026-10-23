/*
 * A140757  Cumulative sums of A140756.
 * https://oeis.org/A140757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,c) where r = smallest integer with r(r+1)/2 >= n and
 *            c = n - r(r-1)/2. Here T(n,k) = floor((n+(-1)^{n-k})^2/4) -
 *            (-1)^{n-k}*floor((n-k)/2)
 * Range:     u8 n <= 215, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r such that r(r-1)/2 < n <= r(r+1)/2. */
    idx_t r = 1;
    idx_t t = 1;              /* t = r(r+1)/2 after each iteration */
    while (t < n) {
        ++r;
        t += r;
    }

    /* Column c in that row: */
    idx_t prev = t - r;       /* sum of first (r-1) rows: (r-1)r/2 */
    idx_t c = n - prev;

    /* Compute T(r,c). Use signed arithmetic for the sign. */
    long long diff = (long long)r - (long long)c;          /* n-k */
    int sign = (diff & 1LL) ? -1 : 1;                     /* (-1)^{n-k} */

    /* (n + (-1)^{n-k})^2 / 4 */
    long long s = (long long)r + (long long)sign;          /* r + sign */
    seq_t part1 = ((seq_t)(s * s)) / (seq_t)4;

    /* floor((n-k)/2) */
    seq_t half = (seq_t)(diff / 2);

    /* Combine with sign */
    seq_t result;
    if (sign == 1) {
        result = part1 - half;
    } else {
        result = part1 + half;
    }

    return result;
}
