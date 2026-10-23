/*
 * A131817  a(n) = A051340(n) + A130321(n) - A000012(n).
 * https://oeis.org/A131817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A051340(n) + A130321(n) - A000012(n) Triangle
 *            representation: for n in row r with offset k (0 ≤ k ≤ r) a(n) =
 *            2^(r−k) if k < r r + 1 if k == r (see first rows: 1; 2,2; 4,2,3;
 *            …)
 * Range:     u8 n <= 35, u16 n <= 135, u32 n <= 527, u64 n <= 2079,
 *            u128 n <= 8255, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t r;                     /* row number */
    /* Find r such that triangular(r) ≤ n < triangular(r+1) */
    if (n == 0) {
        r = 0;
    } else {
        long double approx = sqrt(8.0L * (long double)n + 1.0L);
        r = (idx_t)((approx - 1.0L) / 2.0L);

        /* adjust for any floating‑point inaccuracy */
        while ((unsigned __int128)r * (r + 1) / 2 > n)
            --r;
        while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n)
            ++r;
    }

    unsigned __int128 tri = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)tri;    /* column index within the row */

    if (k < r) {                 /* power of two */
        return pow2(r - k);
    } else {                     /* diagonal entry */
        return (seq_t)(r + 1);
    }
}
