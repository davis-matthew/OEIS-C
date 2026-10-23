/*
 * A143468  Triangle read by rows, A054525 * A127775, 1<=k<=n.
 * https://oeis.org/A143468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   * a(n) = 0 if k does not divide r * a(n) = mobius(r/k) * (2*k -
 *            1) otherwise * * where r and k are obtained from the linear
 *            index n by converting to * row‑major triangle ordering: n =
 *            r(r-1)/2 + k (1 ≤ k ≤ r)
 * Range:     i8 n <= 66, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* not used – offset is 1 */

    /* Find row r such that r(r+1)/2 ≥ n and (r-1)r/2 < n */
    idx_t r = (idx_t)floor((sqrt(8.0 * (double)n + 1.0) - 1.0) / 2.0);
    if (r * (r + 1) / 2 < n) {
        ++r;
    } else {
        /* guard against rare rounding down */
        while (r > 0 && (r - 1) * r / 2 >= n) --r;
    }

    /* Column k in that row */
    idx_t k = n - r * (r - 1) / 2;

    /* If k is not a divisor of r the entry is zero */
    if (r % k != 0) return 0;

    idx_t m = r / k;                       /* divisor that determines μ(m) */
    seq_t mu = mobius(m);                  /* μ(m) ∈ {–1,0,1} */
    if (mu == 0) return 0;

    seq_t odd = (seq_t)(2 * k - 1);         /* odd number 1,3,5,… */
    return mu * odd;
}
