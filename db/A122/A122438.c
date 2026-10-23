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
 * Helpers:   binomial, floor_sqrt, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/floor_sqrt.h"

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
