/*
 * A195310  Triangle read by rows with T(n,k) = n - A001318(k), n >= 1, k >=
 *          1, if (n - A001318(k)) >= 0.
 * https://oeis.org/A195310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = row - A001318(col) where (row, col) are obtained by
 *            reading the triangle T(r,k) = r - A001318(k) (for all k with
 *            A001318(k) <= r) row by row. The sequence is thus the
 *            concatenation of all rows of that triangle
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* A001318(k) : the sequence of absolute generalized pentagonal numbers
   (1,2,5,7,12,15,22,26,...) */
static inline idx_t a001318(idx_t k)
{
    /* k = 1 -> 1, 2 -> 2, 3 -> 5, 4 -> 7, ... */
    idx_t t = (k + 1) / 2;          /* t >= 1  for k >= 1 */
    if (k & 1) { /* odd */
        /* t*(3*t-1)/2 */
        return (idx_t)((__int128)t * (3 * t - 1) / 2);
    } else {          /* even */
        /* t*(3*t+1)/2 */
        return (idx_t)((__int128)t * (3 * t + 1) / 2);
    }
}

/* length of row r = number of k such that A001318(k) <= r */
static inline idx_t row_len(idx_t r)
{
    idx_t k = 0;
    while (1) {
        ++k;
        if (a001318(k) > r)
            break;
    }
    return k - 1;
}

/* Main function required by the build system */
seq_t a(idx_t n)
{
    /* Find the row r that contains the n‑th term (1‑based) */
    idx_t cumulative = 0;
    idx_t r = 1;
    idx_t len = 0;
    while (1) {
        len = row_len(r);
        cumulative += len;
        if (cumulative >= n)
            break;
        ++r;
    }

    /* Position inside the row (0‑based) */
    idx_t prev_cum = cumulative - len;
    idx_t pos = n - prev_cum - 1;   /* 0 <= pos < len */
    idx_t k = pos + 1;              /* 1‑based column index */

    /* Compute the value: row minus A001318(col) */
    seq_t val = (seq_t)r - (seq_t)a001318(k);
    return val;
}
