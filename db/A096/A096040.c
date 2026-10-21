/*
 * A096040  Triangle read by rows: T(n,k) = (n+1,k)-th element of (M^6-M)/5,
 *          where M is the infinite lower Pascal's triangle matrix, 1<=k<=n.
 * https://oeis.org/A096040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = binom(n,k-1)*(6^(n+1-k)-1)/5, a(n) is the triangle read
 *            by rows
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1 */

    /* Find the row r such that r*(r+1)/2 >= n and (r-1)*r/2 < n */
    idx_t low = 0, high = 1;
    while ( ( (__uint128_t)high * (high + 1) ) / 2 < n ) {
        high <<= 1;
    }
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if ( ( (__uint128_t)mid * (mid + 1) ) / 2 >= n )
            high = mid;
        else
            low = mid;
    }
    idx_t r = high;

    /* Previous triangular number */
    idx_t prev_tri = (idx_t)( ( (__uint128_t)(r - 1) * r ) / 2 );

    /* Column index within the row */
    idx_t col = n - prev_tri;   /* 1 <= col <= r */

    /* Compute binomial(r, col-1) */
    seq_t bin = binomial(r, col - 1);

    /* Compute (6^(r+1-col) - 1) / 5 */
    idx_t exp = r + 1 - col;
    seq_t pow6 = big_pow((seq_t)6, (seq_t)exp);
    seq_t part = (pow6 - (seq_t)1) / (seq_t)5;

    /* Final result */
    return bin * part;
}
