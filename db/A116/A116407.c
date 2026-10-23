/*
 * A116407  Triangle whose k-th column has e.g.f. exp(x)*sum{j=0..k,
 *          (-1)^j*Bessel_I(k+j,2x)}.
 * https://oeis.org/A116407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the n‑th term in the triangle whose k‑th column has
 *            ordinary generating function exp(x) * Σ_{j=0..k} (-1)^j
 *            I_{k+j}(2x). The terms are read by rows, i.e. a(n) = T(row, col)
 *            where row = ⌊(√(8n+1)-1)/2⌋ and col = n - row·(row+1)/2
 * Range:     u8 n <= 27, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"

/* integer square root of a 64‑bit unsigned value */
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = x;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq <= x) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* Determine the row r and column c in the triangle */
    uint64_t t = isqrt_u64((uint64_t)8 * n + 1);
    uint64_t r = (t - 1) / 2;                     /* row index, 0‑based   */
    uint64_t tri = r * (r + 1) / 2;                /* cumulative entries before this row */
    uint64_t c = n - tri;                          /* column index               */

    /* The triangle entry T(r, c) is given by
       T(r, c) = Σ_{j=0..c} (-1)^j * B_{c+j}(r)
       where B_m(r) = Σ_{t=0}^{⌊(r-m)/2⌋} binom(r, t) * binom(r-t, t+m)   */
    seq_t sum_pos = 0;     /* Σ over even j  */
    seq_t sum_neg = 0;     /* Σ over odd  j  */

    for (uint64_t j = 0; j <= c; ++j) {
        uint64_t m = c + j;                     /* m = c + j   */
        if (m > r) continue;                    /* B_m(r) = 0 when m > r */
        seq_t term = 0;
        for (uint64_t t = 0; ; ++t) {
            int64_t rem = (int64_t)r - (int64_t)m - 2 * (int64_t)t;
            if (rem < 0) break;
            seq_t bin1 = binom(r, t);
            seq_t bin2 = binom(r - t, t + m);
            term += bin1 * bin2;
        }
        if ((j & 1) == 0)
            sum_pos += term;
        else
            sum_neg += term;
    }

    /* The result is non‑negative; guaranteed by the combinatorial definition. */
    return sum_pos - sum_neg;
}
