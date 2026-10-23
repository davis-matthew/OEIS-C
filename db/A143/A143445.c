/*
 * A143445  Triangle read by rows, A051731 * (A001318 * 0^(n-k)); 1<=k<=n.
 * https://oeis.org/A143445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (n-1)(n)/2 is the triangular number for row index r,
 *            with: a(n) = 0 if r % c != 0 else A001318(c) where r = row
 *            containing linear index n, c = column (1‑based) in that row.
 *            A001318(n) is given by: if n is odd: ((n+1)/2)*(3*(n+1)/2-1)/2
 *            if n is even: (3*m*m - 5*m + 2)/2 with m = n/2 + 1
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   A001318
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include "../utils/A001318.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n==0, but guard */

    /* Find row r such that T(r-1) < n <= T(r),  T(r)=r*(r+1)/2. */
    idx_t r = (idx_t)((sqrt(8.0 * (double)n + 1.0) - 1.0) / 2.0);
    /* Adjust r in case of rounding errors. */
    while (((seq_t)r * (r + 1)) / 2 < n) r++;
    while (r > 1 && ((seq_t)(r - 1) * r) / 2 >= n) r--;

    /* Column (1‑based) within this row. */
    idx_t prevTri = (r - 1) * r / 2;
    idx_t c = n - prevTri; /* 1 <= c <= r */

    if (r % c == 0)
        return A001318(c);
    else
        return 0;
}
