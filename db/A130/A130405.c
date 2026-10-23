/*
 * A130405  Triangle where g.f. of row n = Product_{i=0..n} [F(i+1) + F(i)*x]
 *          for n>=0, where F(i) = A000045(i) is the i-th Fibonacci number.
 * https://oeis.org/A130405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the coefficient of x^k in the product ∏_{i=0}^{r}
 *            (F_{i+1}+F_i·x), where r and k satisfy r·(r+1)/2 ≤ n <
 *            (r+1)(r+2)/2 and k = n - r·(r+1)/2
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/fib.h"

static seq_t **_rows = NULL;      /* cache of rows (each row is an array of seq_t) */
static size_t _row_count = 0;     /* number of rows currently cached */

seq_t a(idx_t n)
{
    /* Determine row r that contains the n-th term. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Position of the term within row r. */
    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;

    /* Build all rows up to r that are not yet cached. */
    for (size_t i = _row_count; i <= (size_t)r; ++i) {
        size_t len = i + 1;                      /* number of coefficients in row i */
        seq_t *row = (seq_t *)calloc(len, sizeof(seq_t));
        if (!row)
            return (seq_t)0;                      /* memory allocation failed */

        if (i == 0) {                            /* row 0 is just the constant 1 */
            row[0] = (seq_t)1;
        } else {
            seq_t ai = fib((idx_t)i);            /* F(i)   */
            seq_t bi = fib((idx_t)(i + 1));      /* F(i+1) */
            seq_t *prev = _rows[i - 1];          /* previous row */

            row[0] = prev[0] * bi;
            for (size_t j = 1; j < i; ++j)
                row[j] = prev[j] * bi + prev[j - 1] * ai;
            row[i] = prev[i - 1] * ai;
        }

        /* Store the new row in the cache. */
        _rows = (seq_t **)realloc(_rows, (i + 1) * sizeof(seq_t *));
        if (!_rows)
            return (seq_t)0;                      /* memory allocation failed */
        _rows[i] = row;
        _row_count = i + 1;
    }

    return _rows[(size_t)r][(size_t)k];
}
