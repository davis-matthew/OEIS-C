/*
 * A009998  Triangle in which j-th entry in i-th row is (j+1)^(i-j).
 * https://oeis.org/A009998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, easy, nice
 * Formula:   a(n) = (k+1)^(r-k) where * r = smallest non‑negative integer
 *            with (r+1)(r+2)/2 > n * k = n - r(r+1)/2 (0 ≤ k ≤ r) * This is
 *            the flattening by rows of the triangle T(i,j) = (j+1)^(i-j)
 * Range:     u8 n <= 30, u16 n <= 68, u32 n <= 174, u64 n <= 440,
 *            u128 n <= 1138, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find the row r such that the cumulative number of terms up to
     * row r-1 is ≤ n < the number up to row r.  The cumulative count
     * up to row r is (r+1)(r+2)/2. */
    idx_t r = 0;
    while (((r + 1) * (r + 2) / 2) <= n) {
        r++;
    }

    /* Index k of the term within the row. */
    idx_t prev_sum = r * (r + 1) / 2;      /* sum of terms in rows < r */
    idx_t k = n - prev_sum;                /* 0 ≤ k ≤ r */

    /* Compute (k+1)^(r-k) using the provided arbitrary‑precision power. */
    seq_t base = (seq_t)(k + 1);
    seq_t exp  = (seq_t)(r - k);
    return big_pow(base, exp);
}
