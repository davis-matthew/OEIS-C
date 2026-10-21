/*
 * A240223  Rectangular companion array to M(n,k), given in A240222, showing
 *          the end numbers N(n, k), k >= 1, for the Collatz operation
 *          (udd)^(n-1) ud, n >= 1, read by antidiagonals.
 * https://oeis.org/A240223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is obtained by reading the array N(m,k)=2+3^m*k
 *            antidiagonally. For a given index n≥0 it is:
 *            a(n)=2+3^row*(d-row) where d = ⌊(√(8n+1)-1)/2⌋+1 and row = n-
 *            (d-1)d/2 +1
 * Range:     u8 n <= 24, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_int.h"

/* Integer square root for unsigned __int128 values.  */
static inline uint64_t isqrt_uint128(unsigned __int128 x)
{
    unsigned __int128 lo = 0, hi = ((unsigned __int128)1 << 34);
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi + 1) >> 1;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return (uint64_t)lo;
}

seq_t a(idx_t n)
{
    /* Determine diagonal d.                               */
    unsigned __int128 t = (unsigned __int128)1 + ((unsigned __int128)8 * (unsigned __int128)n);
    uint64_t sqrt_val = isqrt_uint128(t);
    uint64_t d = (sqrt_val - 1) / 2 + 1;

    /* Position within the diagonal.                       */
    unsigned __int128 prev = ((unsigned __int128)(d - 1) * d) / 2;
    unsigned __int128 pos128 = (unsigned __int128)n - prev;
    uint64_t pos = (uint64_t)pos128;            /* 0‑based index inside diagonal */

    /* Convert to array coordinates.                       */
    idx_t row = (idx_t)(pos + 1);               /* m coordinate (>=1)           */
    idx_t k   = (idx_t)(d - row);               /* k coordinate (>=0)           */

    /* Compute N(row, k) = 2 + 3^row * k.                */
    seq_t val = pow_int((seq_t)3, row);
    val = val * ((seq_t)k) + (seq_t)2;
    return val;
}
