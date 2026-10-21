/*
 * A224380  Table read by antidiagonals of numbers of form (2^n -1)*2^(m+2) +
 *          3 where n>=1, m>=1.
 * https://oeis.org/A224380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, nonn
 * Formula:   a(n) = (2^i - 1) * 2^(j+2) + 3, where (i, j) are the row and
 *            column indices obtained by traversing the table of numbers (2^i
 *            - 1) × 2^(j+2) along anti‑diagonals
 * Range:     u8 n <= 15, u16 n <= 91, u32 n <= 435, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* below the offset; undefined behavior allowed */

    /* Find the anti‑diagonal number s such that s(s-1)/2 >= n. */
    idx_t s = 2;
    while ((s - 1) * s / 2 < n) ++s;

    /* Number of terms before this diagonal. */
    idx_t prev = (s - 2) * (s - 1) / 2;

    /* Row (i) and column (j) indices (1‑based). */
    idx_t i = n - prev;      /* 1 <= i < s */
    idx_t j = s - i;         /* 1 <= j < s */

    /* Compute the table entry: (2^i - 1) * 2^(j+2) + 3. */
    seq_t row = pow2(i);          /* 2^i   */
    row -= (seq_t)1;              /* 2^i-1 */
    seq_t col = pow2(j + 2);      /* 2^(j+2) */
    seq_t val = row * col + (seq_t)3;
    return val;
}
