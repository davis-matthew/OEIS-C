/*
 * A230208  Trapezoid of dot products of row 5 (signs alternating) with
 *          sequential 6-tuples read by rows in Pascal's triangle A007318:
 *          T(n,k) is the linear combination of the 6-tuples (C(5,0), -C(5,1),
 *          ..., -C(5,5)) and (C(n-1,k-5), C(n-1,k-4), ..., C(n-1,k)), n >= 1,
 *          0 <= k <= n+4.
 * https://oeis.org/A230208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign, tabf
 * Formula:   a(n) is obtained by reading the trapezoid T from left to right,
 *            row by row. Each element of the trapezoid is T(n,k) =
 *            Σ_{i=0}^{5} (-1)^{i+1} C(5,i) · C(n-1, k-i), where 1 ≤ n and 0 ≤
 *            k ≤ n+4. This implementation maps the flat index n to (row,
 *            column) and evaluates T(n,k) using a fast binomial rou...
 * Range:     i8 n <= 144, i16 n <= 247, i32 n <= 1032, i64 n <= 1525,
 *            i128 n <= 1525, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r such that the cumulative number of entries up
       to that row is at least n.  Row r has (r+5) entries. */
    idx_t r = 1;
    while ((unsigned __int128)r * (r + 11) / 2 < n)
        ++r;

    /* Number of entries before the current row */
    const idx_t rows_before = r - 1;
    const unsigned __int128 cum_prev_val =
        (unsigned __int128)rows_before * (rows_before + 11) / 2;
    const idx_t cum_prev = (idx_t)cum_prev_val;

    /* Position k within the current row (0‑based) */
    const idx_t k = n - cum_prev - 1;

    /* Pre‑computed binomial coefficients C(5,i) */
    static const seq_t C5[6] = {1, 5, 10, 10, 5, 1};

    seq_t sum = 0;
    /* Compute T(r,k) = Σ (-1)^{i+1} C(5,i) * C(r-1, k-i) */
    for (idx_t i = 0; i < 6; ++i) {
        if (i > k)            /* k-i would underflow */
            continue;
        const idx_t t = k - i;
        if (t > r - 1)        /* C(r-1, t) is zero otherwise */
            continue;
        seq_t bin = binomial(r - 1, t);      /* C(r-1, k-i) */
        seq_t term = bin * C5[i];             /* |C(5,i) * C| */
        if ((i & 1) == 0)                  /* i even → sign = - */
            sum -= term;
        else                               /* i odd  → sign = + */
            sum += term;
    }
    return sum;
}
