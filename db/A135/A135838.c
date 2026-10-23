/*
 * A135838  Triangle read by rows: T(n,k) = 2^floor(n/2)*binomial(n-1,k-1).
 * https://oeis.org/A135838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 2^⌊row/2⌋ * binomial(row-1, col-1), where (row, col) is
 *            the position of the n‑th element in the triangle T(m,k) =
 *            2^⌊m/2⌋ * binomial(m-1,k-1) read row by row. Indices are 1‑based
 * Range:     u8 n <= 30, u16 n <= 95, u32 n <= 286, u64 n <= 1054,
 *            u128 n <= 3957, big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/binomial.h"

/* Find the row r such that 1 + 2 + … + r  >=  n            (r >= 1)   */
static idx_t find_row(idx_t n)
{
    if (n < 2) return 1;
    idx_t high = 1;
    /* double high until the triangular number is large enough */
    while ((seq_t)high * (high + 1) / 2 < (seq_t)n)
        high <<= 1;            /* high *= 2 */

    idx_t low = 1;
    /* binary search in [low,high] */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((seq_t)mid * (mid + 1) / 2 >= (seq_t)n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Return the n-th term of A135838.  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* the sequence is 1‑based */

    idx_t row = find_row(n);
    /* number of entries before the current row */
    idx_t prev_total = row > 1 ? (idx_t)((__uint128_t)(row - 1) * row / 2) : 0;
    idx_t col = n - prev_total;               /* 1‑based column index */

    seq_t pow_term = pow2(row >> 1);          /* 2^⌊row/2⌋ */
    seq_t bin_coeff = binomial(row - 1, col - 1);   /* C(row-1, col-1) */

    return pow_term * bin_coeff;
}
