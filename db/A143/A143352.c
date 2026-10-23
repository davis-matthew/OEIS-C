/*
 * A143352  Triangle read by rows, A051731 * A054524 = (A051731)^2 * A128407;
 *          1<=k<=n.
 * https://oeis.org/A143352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = divisor_count(r / k) * mu(k) if k divides r (where r =
 *            row index and k = column in the triangle read row‑wise);
 *            otherwise a(n) = 0. This implements OEIS A143352
 * Range:     i8 n <= 105, i16 n <= 105, i32 n <= 105, i64 n <= 105,
 *            i128 n <= 105, big any n
 * Helpers:   divisor_count_u64, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 >= n and (r-1)*r/2 < n.  */
    idx_t low  = 1;
    idx_t high = n;                       /* valid for n >= 1                */
    while (low < high)
    {
        idx_t mid = (low + high) / 2;
        /* Use 128‑bit arithmetic to avoid overflow.             */
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low; /* 1‑based row index */

    /* Compute column index k within the row (1‑based).         */
    __uint128_t prev = (__uint128_t)r * (r - 1) / 2;
    idx_t k = n - (idx_t)prev;                 /* 1 <= k <= r            */

    if (r % k != 0)
        return (seq_t)0;                       /* entry is zero           */

    idx_t m = r / k;                           /* integer division       */

    /* Compute d(m) * mu(k).  divisor_count_u64 expects uint64_t.  */
    seq_t divcnt = (seq_t)divisor_count_u64((uint64_t)m);
    seq_t mu     = mobius(k);

    return divcnt * mu;
}
