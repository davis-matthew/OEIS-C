/*
 * A140586  Triangle t(n,m) read by rows: t(n,m) = binomial(n,m) if m <=
 *          floor(n/3) or m >= floor(2n/3), otherwise t(n,m)=0.
 * https://oeis.org/A140586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = flattening of triangle t(n,m) where t(n,m) = C(n,m) if m
 *            <= floor(n/3) or m >= floor(2n/3), else 0
 * Range:     u8 n <= 58, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* offset starts at 1 */

    /* Find the row r such that r(r+1)/2 < n <= (r+1)(r+2)/2. */
    idx_t lo = 0;
    idx_t hi = 1;

    /* Increase hi until (hi+1)(hi+2)/2 >= n.  Use 128‑bit to avoid overflow. */
    while ((unsigned __int128)(hi + 1) * (hi + 2) / 2 < n) {
        if (hi > ((idx_t)(~0ULL) >> 1))
            break; /* safety guard for extremely large n */
        hi <<= 1;
    }

    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 cum = (unsigned __int128)(mid + 1) * (mid + 2) / 2;
        if (cum >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;                       /* current row index (0‑based) */

    unsigned __int128 before = (unsigned __int128)r * (r + 1) / 2;
    idx_t m = n - (idx_t)before - 1;     /* position within the row (0‑based) */

    idx_t floor_n_div3 = r / 3;
    idx_t floor_2n_div3 = (2 * r) / 3;

    if (m <= floor_n_div3 || m >= floor_2n_div3)
        return binomial(r, m);
    else
        return (seq_t)0;
}
