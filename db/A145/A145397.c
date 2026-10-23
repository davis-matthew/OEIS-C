/*
 * A145397  Numbers not of the form m*(m+1)*(m+2)/6, the non-tetrahedral
 *          numbers.
 * https://oeis.org/A145397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n+m if 6*(n+m) > m*(m+1)*(m+2), otherwise a(n)=n+m-1,
 *            where m = floor((6*n)^(1/3)). The index n starts at 1
 * Range:     u8 n <= 245, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   cube_root_idx_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/cube_root_idx_t.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n=0; safe default */

    /* m = floor((6*n)^(1/3)) */
    idx_t six_n = 6 * n;               /* safe for n up to 10⁴ → < 2⁶⁴   */
    idx_t m = cube_root_idx_t(six_n);

    /* Compare 6*(n+m) and m*(m+1)*(m+2) using 128‑bit arithmetic */
    unsigned __int128 lhs = (unsigned __int128)6 * (n + m);
    unsigned __int128 rhs = (unsigned __int128)m * (m + 1) * (m + 2);

    if (lhs > rhs)
        return (seq_t)(n + m);
    else
        return (seq_t)(n + m - 1);
}
