/*
 * A162990  Triangle of polynomial coefficients related to
 *          3F2([1,n+1,n+1],[n+2,n+2],z).
 * https://oeis.org/A162990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = ((k+1)! / i)^2 where k is the row number such that
 *            k(k+1)/2 ≥ n and i = n – k(k−1)/2 (the column within that row)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 28, u64 n <= 66, u128 n <= 190,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                /* undefined for offset 0 */

    /* Find the row number k such that k(k+1)/2 ≥ n.  */
    idx_t low  = 1;
    idx_t high = 1;
    while (high * (high + 1) / 2 < n)          /* grow upper bound */
        high <<= 1;                            /* double each time */

    while (low < high) {                      /* binary search */
        idx_t mid = low + (high - low) / 2;
        if (mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t k = low;                            /* row number */

    idx_t prev = k * (k - 1) / 2;              /* elements before row k */
    idx_t i = n - prev;                       /* column within the row (1‑based) */

    /* Compute ((k+1)! / i)^2 .  */
    seq_t fac = factorial(k + 1);             /* (k+1)! */
    seq_t q   = fac / (seq_t)i;               /* integer division, exact */
    return q * q;                             /* square the quotient */
}
