/*
 * A225816  Square array A(n,k) = (k!)^n, n>=0, k>=0, read by antidiagonals.
 * https://oeis.org/A225816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   The square array A(n,k) = (k!)^n is read by antidiagonals. For a
 *            given index n (starting at 0) let s be the smallest integer such
 *            that (s+1)(s+2)/2 > n. Let offset = n - s*(s+1)/2; // 0 <=
 *            offset <= s Then the coordinates of a(n) are i = offset (row), k
 *            = s - offset (column). The value is a(n) =...
 * Range:     u8 n <= 22, u16 n <= 37, u32 n <= 57, u64 n <= 93, u128 n <= 141,
 *            big any n
 * Validated: standard, pass, 71 terms through n = 626 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    /* Find the diagonal index s such that n lies in it. */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 <= n)
        ++s;

    /* Position inside the diagonal. */
    idx_t offset = n - s * (s + 1) / 2;   // i
    idx_t i = offset;
    idx_t k = s - offset;

    /* Compute (k!)^i. */
    seq_t base = factorial(k);       /* k!  */
    return ipow_idx(base, i);          /* (k!)^i */
}
