/*
 * A225213  Triangular array read by rows. T(n,k) is the number of cycles in
 *          the digraph representation of all functions
 *          f:{1,2,...,n}->{1,2,...,n} that have length k; 1<=k<=n.
 * https://oeis.org/A225213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) = (k-1)! * C(r,k) * r^(r-k) where r is the row
 *            such that n lies in the r‑th row when the triangular array
 *            T(n,k) is read by rows (OEIS A225213)
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   binomial, factorial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"
#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 >= n */
    idx_t r_low = 1;
    idx_t r_high = n;     /* r never exceeds n for n>=1 */
    while (r_low < r_high) {
        idx_t mid = (r_low + r_high) >> 1;
        idx_t lhs = mid * (mid + 1) / 2;
        if (lhs >= n)
            r_high = mid;
        else
            r_low = mid + 1;
    }
    idx_t r = r_low;

    /* Column index k */
    idx_t prev = (r - 1) * r / 2;
    idx_t k = n - prev;   /* 1 <= k <= r */

    /* Compute (k-1)! * binomial(r,k) * r^(r-k) */
    seq_t fact = factorial(k - 1);          /* (k-1)!   */
    seq_t bin  = binomial(r, k);             /* C(r,k)   */
    seq_t pw   = pow_uint((uint64_t)r, r - k);   /* r^(r-k)  */

    seq_t result = fact;
    result = result * bin;
    result = result * pw;
    return result;
}
