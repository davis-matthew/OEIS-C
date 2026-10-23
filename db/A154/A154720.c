/*
 * A154720  Triangle read by rows in which row n lists 2n-1 terms: n, in the
 *          center of the row and all the pairs of noncomposite numbers
 *          equidistant to n, with 0's inserted, as shown below in the
 *          example.
 * https://oeis.org/A154720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) is obtained by reading a triangle of width 2r‑1 where row r
 *            (r ≥ 1) contains numbers r−(r−1) … r+(r−1). For a distance d
 *            from the centre (1 ≤ d < r) the numbers r−d and r+d are written
 *            side‑by‑side only if both are noncomposite (1 or prime);
 *            otherwise zeroes are written. The sequence lists t...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* Find the row r such that (r−1)² < n ≤ r² */
    idx_t r = _floor_isqrt_u64(n - 1) + 1;               /* r ≥ 1 */
    idx_t prev_sq = (r - 1) * (r - 1);                   /* (r‑1)² */
    idx_t pos = n - prev_sq;                             /* 1‑based position in the row */

    if (pos == r)                                         /* centre element */
        return (seq_t)r;

    idx_t d = (pos > r) ? (pos - r) : (r - pos);          /* distance from centre */
    idx_t left  = r - d;                                  /* number at left side      */
    idx_t right = r + d;                                  /* number at right side     */

    /* Check whether both sides are noncomposite (1 or prime) */
    bool left_ok  = (left  == 1) || is_prime_small(left);
    bool right_ok = (right == 1) || is_prime_small(right);

    if (!left_ok || !right_ok)
        return (seq_t)0;                                   /* at least one side composite */

    /* Return the appropriate side based on the position */
    return (pos < r) ? (seq_t)left : (seq_t)right;
}
