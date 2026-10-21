/*
 * A099510  Triangle, read by rows, of trinomial coefficients arranged so that
 *          there are n+1 terms in row n by setting T(n,k) equal to the
 *          coefficient of z^k in (1 + 2*z + z^2)^(n-[k/2]), for n>=k>=0,
 *          where [k/2] is the integer floor of k/2.
 * https://oeis.org/A099510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A099510: a(n) is the (n)th term of the triangle
   read by rows, where T(r,k)=binomial(2*r-2*floor(k/2),k) with r = floor
  ((sqrt(8*n+1)-3)/2) and k = n - r*(r+1)/2. */
#include <stdint.h>
#include "../utils/binomial.h"

/* seq_t and idx_t are provided by the build. */
seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= (unsigned __int128)n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    idx_t r = high;                          /* row index   */
    idx_t start = r * (r + 1) / 2;            /* first index of row r */
    idx_t k = n - start;                      /* column index within the row */
    idx_t f = k >> 1;                         /* floor(k/2)  */
    idx_t m = 2 * r - 2 * f;                  /* exponent for binomial */
    return binomial(m, k);
}
