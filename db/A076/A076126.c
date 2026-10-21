/*
 * A076126  Triangle T(n,k) of associated Lah numbers, n>=2, k=1..floor(n/2).
 * https://oeis.org/A076126
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) returns the n‑th entry of the triangle * T(n,k) = n!/k! *
 *            binomial(n-k-1, k-1) (n ≥ 2, 1 ≤ k ≤ ⌊n/2⌋), * where the
 *            triangle is read row‑wise with rows starting at n = 2 * and the
 *            sequence offset is 2 (so a(2) is the first entry)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   binom_mult
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom_mult.h"

seq_t a(idx_t n)
{
    /* Sequence is defined only for n >= 2.  Below offset return 0. */
    if (n < 2) return (seq_t)0;

    /* Convert to 0‑based index relative to the first term a(2). */
    idx_t m = n - 2;

    /* Find the row number (starting at 2) and the position within that row. */
    idx_t row = 2;      /* current row number */
    idx_t acc = 0;      /* number of terms before the current row */

    while (true) {
        idx_t len = row >> 1;          /* floor(row/2) */
        if (acc + len > m) break;      /* the desired term is here */
        acc += len;
        ++row;
    }

    idx_t pos = m - acc;      /* 0‑based column index in this row */
    idx_t k   = pos + 1;      /* 1‑based column (list) index */

    /* Compute (row! / k!) as the product (k+1)…row. */
    seq_t prod = (seq_t)1;
    for (idx_t i = k + 1; i <= row; ++i) {
        prod *= (seq_t)i;
    }

    /* Compute binomial(row-k-1, k-1). For k = 1 this is 1. */
    seq_t binom = binom_mult(row - k - 1, k - 1);

    /* Return the associated Lah number. */
    return prod * binom;
}
