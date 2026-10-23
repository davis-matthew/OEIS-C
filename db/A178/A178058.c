/*
 * A178058  Number of 1's in the Gray code for binomial(n,m).
 * https://oeis.org/A178058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = popcount( gray( C(i,j) ) ) * where (i,j) are determined
 *            by the standard triangular * enumeration of the upper‑triangular
 *            matrix: * i = floor( (sqrt(8n+1)-1)/2 ) * j = n - i(i+1)/2 * and
 *            gray(x) = x ^ (x >> 1)
 * Range:     u8 n <= 39, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

static inline idx_t antidiagonal_row(idx_t n)
{
    /* Find the largest i such that i*(i+1)/2 <= n.
       Uses binary search on the 0..n range; 128‑bit arithmetic
       avoids overflow in the triangular calculation. */
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;   /* upper mid */
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    idx_t i = antidiagonal_row(n);
    idx_t tri = (idx_t)(((__int128)i * (i + 1)) / 2);
    idx_t j = n - tri;                /* 0 <= j <= i */

    seq_t val   = binomial(i, j);      /* C(i,j) */
    seq_t gray  = val ^ (val >> 1);    /* Gray code of the binomial */

    seq_t count = 0;
    while (gray) {
        count++;
        gray &= gray - 1;              /* clear lowest set bit */
    }
    return count;
}
