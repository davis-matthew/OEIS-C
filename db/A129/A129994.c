/*
 * A129994  Triangle read by rows: 2*A007318 - I.
 * https://oeis.org/A129994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2*binom(r,c) - (c==r ? 1 : 0) where r is the unique row
 *            such that r*(r+1)/2 <= n < (r+1)*(r+2)/2, and c = n - r*(r+1)/2
 *            (0‑based column)
 * Range:     u8 n <= 39, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the starting index of row r:  r*(r+1)/2. */
static inline idx_t _row_start(idx_t r)
{
    /* Use 128‑bit arithmetic to avoid overflow for large r. */
    return (idx_t)(((__uint128_t)r * (r + 1)) / 2);
}

seq_t a(idx_t n)
{
    /* Find row r by incrementing until the start of the next row
       exceeds n.  For n up to 10 000 this loop runs at most ~140
       times – far below the time limit. */
    idx_t r = 0;
    while (_row_start(r + 1) <= n)
        ++r;

    idx_t offset = _row_start(r);
    idx_t c = n - offset;           /* column index in row r (0‑based) */

    /* Compute binomial coefficient binom(r,c).
       Use symmetry to reduce the number of iterations. */
    idx_t k = c;
    if (k > r - k)
        k = r - k;                    /* k = min(c, r-c) */

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(r - k + i);   /* multiply by numerator */
        res = res / (seq_t)i;             /* divide by denominator */
    }

    /* Apply the transformation 2*P - I */
    res = res * 2;
    if (c == r)
        res = res - 1;

    return res;
}
