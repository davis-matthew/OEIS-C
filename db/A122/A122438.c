/*
 * A122438  Riordan array (1/(1-2x), x(1+2x)).
 * https://oeis.org/A122438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the n-th entry of the Riordan array triangle with
 *            generating functions (1/(1-2x), x(1+2x)), read row by row. For
 *            n, let r = floor((sqrt(1+8n)-1)/2) and offset = r(r+1)/2. Then m
 *            = n - offset, where r is the row index and m the column. The
 *            triangle entry is T(r,m) = 2^(r-m) * sum_{t=0}^{r-...
 * Range:     u8 n <= 7, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 63 (2026-11-01)
 * Helpers:   binomial, ipow_i32, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow_i32.h"
#include "../utils/pow2.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    seq_t val = (seq_t)1 + (seq_t)8 * n;      /* 1 + 8*n  */
    seq_t sq = floor_sqrt(val);               /* sqrt(1+8n) */
    idx_t r = (idx_t)((sq - 1) / 2);          /* floor((sqrt-1)/2) */

    idx_t offset = r * (r + 1) / 2;          /* starting index of row r */
    idx_t m = n - offset;                    /* column index in the row */
    idx_t exp = r - m;                       /* power exponent for 2 */

    seq_t power = pow2(exp);                  /* 2^(r-m) */

    seq_t sum = 0;
    for (idx_t t = 0; t <= exp; ++t)
    {
        sum += binomial(m, t);
    }

    return power * sum;
}
