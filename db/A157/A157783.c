/*
 * A157783  Triangle read by rows: the coefficient [x^k] of the polynomial
 *          Product_{i=1..n} (3^(i-1)-x) in row n, column k, 0 <= k <= n.
 * https://oeis.org/A157783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the coefficient [x^k] of ∏_{i=1}^r (3^(i-1)-x), where r
 *            = ⌊ (√(8·n+1)–1)/2 ⌋ and k = n – r(r+1)/2. The coefficient is
 *            (-1)^k times the elementary symmetric sum e_{r-k} of
 *            1,3,9,…,3^{r-1}
 * Range:     i8 n <= 9, i16 n <= 14, i32 n <= 27, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find row r: r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column index within the row. */
    idx_t k = n - r * (r + 1) / 2;

    /* Allocate array for elementary symmetric sums up to degree r. */
    seq_t *e = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    if (!e)
        return 0; /* unlikely for the given constraints */

    for (idx_t i = 0; i <= r; ++i)
        e[i] = 0;
    e[0] = 1;                      /* e_0 = 1 by definition */

    /* Iterate over i = 1..r, a_i = 3^(i-1). */
    seq_t val = 1; /* a_1 = 3^0 = 1 */
    for (idx_t i = 0; i < r; ++i) {
        if (i > 0)
            val *= 3; /* now val = a_{i+1} = 3^i */

        /* Update e[j] = e_j + val * e_{j-1} for j from i+1 down to 1. */
        for (idx_t j = i + 1; j > 0; --j)
            e[j] += val * e[j - 1];
    }

    /* The desired coefficient is (-1)^k * e_{r-k}. */
    seq_t coef = e[r - k];
    if (k & 1)
        coef = -coef;

    free(e);
    return coef;
}
