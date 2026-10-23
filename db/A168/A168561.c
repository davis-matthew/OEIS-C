/*
 * A168561  Triangle of coefficients of the Fibonacci polynomials, starting
 *          with F(0, x) = 1, F(1, x) = x.
 * https://oeis.org/A168561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl, changed
 * Formula:   a(n) = T(r,k) where r = ⌊(√(8n+1)−1)/2⌋, * k = n − r(r+1)/2. *
 *            T(r,k) = binomial((r+k)/2 , k) if r+k is even, else 0
 * Range:     u8 n <= 81, u16 n <= 388, u32 n <= 1274, u64 n <= 1274,
 *            u128 n <= 1274, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t low = 0, high = n, r = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri <= ( __int128)n) {
            r = mid;
            low = mid + 1;
        } else
            high = mid - 1;
    }

    /* Column offset within the found row. */
    __int128 tri_r = (__int128)r * (r + 1) / 2;
    idx_t col = (idx_t)(( __int128)n - tri_r);

    /* If r+col is odd, the entry is zero. */
    if (((r + col) & 1) != 0)
        return (seq_t)0;

    /* Compute the binomial coefficient. */
    idx_t sum = (r + col) >> 1;             /* (r+col)/2 */
    return binom(sum, col);
}
