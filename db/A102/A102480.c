/*
 * A102480  Triangle read by rows: row n contains the numbers C(n,k)^(k-1) for
 *          0 <= k <= n, n >= 0.
 * https://oeis.org/A102480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binom(row, col)^(col-1) where row = largest r with
 *            r(r+1)/2 ≤ n and col = n - row(row+1)/2 For col = 0 the value is
 *            defined to be 1
 * Range:     u8 n <= 23, u16 n <= 32, u32 n <= 60, u64 n <= 97, u128 n <= 162,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & (idx_t)1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Find the row index r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = 0;
    /* Use 128‑bit arithmetic to avoid overflow during the search. */
    while ((__uint128_t)(r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t row_start = r * (r + 1) / 2;
    idx_t c = n - row_start;          /* column index within the row */

    if (c == 0)
        return (seq_t)1;               /* binom(r,0)^(−1) is defined as 1 */

    seq_t bin = binom(r, c);           /* C(r, c) */
    return pow_seq(bin, c - 1);        /* raise to (c-1) */
}
