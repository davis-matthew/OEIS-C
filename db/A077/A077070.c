/*
 * A077070  Triangle read by rows: T(n,k) is the power of 2 in denominator of
 *          coefficients of Legendre polynomials, where n >= 0 and 0 <= k <=
 *          n.
 * https://oeis.org/A077070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2*r - popcnt(r-k) - popcnt(k) * where (r,k) is the row
 *            and column of the n‑th term in the triangle T, * defined by
 *            T(n,k)=2*n-popcnt(n-k)-popcnt(k). * The index n is 0‑based and
 *            the triangle is read row by row. * This implementation works for
 *            all build configurations where * idx_t is an un...
 * Range:     u8 n <= 8384, u16 n <= 8876, u32 n <= 8876, u64 n <= 8876,
 *            u128 n <= 8876, big any n
 * Helpers:   popcnt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>         /* for uint64_t if needed */
#include "../utils/popcnt_u64.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Find the smallest r such that (r+1)*(r+2)/2 > n,
     * i.e. n lies in row r (0‑based).  Since n <= 10000,
     * a simple linear scan is more than fast enough. */
    idx_t r = 0;
    while ( ((r + 1) * (r + 2)) / 2 <= n )
        r++;

    /* Number of terms before row r */
    idx_t prev = (r * (r + 1)) / 2;
    /* Column index within the row */
    idx_t k = n - prev;

    /* Compute the value using the defining formula */
    unsigned int w1 = popcnt_u64(r - k);
    unsigned int w2 = popcnt_u64(k);
    idx_t val = 2 * r - w1 - w2;

    return (seq_t)val;
}
