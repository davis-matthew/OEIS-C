/*
 * A106194  Triangle read by rows, generated from binomial transforms of odd
 *          numbers.
 * https://oeis.org/A106194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Σ_{k=c}^{r} binom(r,k)*(2(k-c)+1), where r = row_of(n)
 *            (row index), c = n - r(r+1)/2 (column index)
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 54 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <sys/types.h>
#include "../utils/binomial.h"

static inline idx_t
row_of(idx_t n)                 /* return r with r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
{
    idx_t low  = 0;
    idx_t high = 1;
    /* exponential search to find an upper bound */
    while ((unsigned __int128)high * (high + 1) / 2 <= n) {
        low  = high;
        high <<= 1;              /* double high */
    }
    /* binary search between low (inclusive) and high (inclusive) */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1) + 1; /* upper mid to avoid infinite loop */
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Determine the row r and column c containing index n */
    idx_t r = row_of(n);
    idx_t offset = (idx_t)((unsigned __int128)r * (r + 1) / 2);   /* r*(r+1)/2 */
    idx_t c = n - offset;

    /* Compute the binomial‑transform value for this (r,c) */
    seq_t result = 0;
    for (idx_t k = c; k <= r; ++k) {
        seq_t bin = binomial(r, k);                      /* binomial(r,k)  */
        seq_t coeff = ((seq_t)(k - c) * 2) + 1;        /* 2(k-c)+1      */
        result += bin * coeff;
    }
    return result;
}
