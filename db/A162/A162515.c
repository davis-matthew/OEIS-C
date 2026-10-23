/*
 * A162515  Triangle of coefficients of polynomials defined by Binet form:
 *          P(n,x) = (U^n - L^n)/d, where U = (x + d)/2, L = (x - d)/2, d =
 *          sqrt(x^2 + 4).
 * https://oeis.org/A162515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 392, u32 n <= 1352, u64 n <= 4901,
 *            u128 n <= 5151, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of A162515: coefficients of Fibonacci
   polynomials flattened row­wise.  The triangle T(n,k) is defined by
   T(n,k) = T(n-1,k) + T(n-2,k-2), with T(0,0)=0, T(1,0)=1.
   For a given linear index n, the coefficient is
       a(n) = 0                           if the corresponding column is odd,
       a(n) = binom(r-1-k, k)             otherwise,   r = the row of n
   where binom() is the standard binomial coefficient function from
   <../utils/binom.h>.  See the OEIS entry for the full description. */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* a(0) = 0 */

    /* Find the row r such that start(r) ≤ n < start(r+1),
       where start(r) = r*(r-1)/2 + 1 for r ≥ 1,  start(0) = 0. */
    idx_t low = 0, high = 1;

    /* Grow high until start(high) > n */
    while (1) {
        unsigned __int128 tmp = (high == 0) ? 0
            : (unsigned __int128)high * (high - 1) / 2 + 1;
        if (tmp > (unsigned __int128)n)
            break;
        if (high > ((idx_t)-1) / 2)   /* avoid overflow of high<<=1 */
            break;
        high <<= 1;
    }

    /* Binary search for the correct r */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 mid_start =
            (unsigned __int128)mid * (mid - 1) / 2 + 1;
        if (mid_start <= (unsigned __int128)n)
            low = mid;
        else
            high = mid;
    }
    idx_t r = low;

    unsigned __int128 start_r = (unsigned __int128)r * (r - 1) / 2 + 1;
    idx_t j = n - (idx_t)start_r;          /* column index within row r */

    if (j & 1)                     /* odd column → coefficient is zero */
        return (seq_t)0;

    idx_t k = j >> 1;               /* k = j/2                          */
    idx_t n1 = r - k - 1;           /* upper argument of the binomial    */

    return binom(n1, k);
}
