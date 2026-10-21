/*
 * A223518  Triangle T(n,k) represents the coefficients of (x^16*d/dx)^n,
 *          where n=1,2,3,...; generalization of Stirling numbers of second
 *          kind A008277, Lah-numbers A008297.
 * https://oeis.org/A223518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 21, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle T(n,k) representing the coefficients of (x^16 * d/dx)^n
 * Recurrence:
 *   T(1,1) = 1
 *   T(n,n) = 1                     (1 ≤ n)
 *   T(n,k) = T(n-1,k) * ((n-1)*(16-1)+k) + T(n-1,k-1)   (1 ≤ k ≤ n-1)
 *
 * The OEIS sequence a(n) lists the rows of this triangle in row‑wise order.
 *
 * This implementation lazily builds the triangle up to the required row
 * and caches all computed rows.  It works for any index n ≥ 1 and
 * performs well for n up to at least 10 000.
 */

#include <stdlib.h>

static seq_t **triangle = NULL;      /* rows[1..max_row] (index 0 unused) */
static idx_t triangle_rows = 0;      /* highest row computed */

/* Build triangle rows up to and including row 'r' */
static void ensure_row(idx_t r)
{
    if (triangle_rows >= r)
        return;

    /* Resize the pointer array */
    size_t new_len = (size_t)(r + 1);
    seq_t **new_tri = (seq_t**)realloc(triangle, new_len * sizeof(seq_t*));
    triangle = new_tri;

    /* Compute any missing rows */
    for (idx_t i = triangle_rows + 1; i <= r; ++i)
    {
        seq_t *row = (seq_t*)malloc((size_t)(i + 1) * sizeof(seq_t));
        /* row[0] is unused */
        if (i == 1) {
            row[1] = 1;          /* T(1,1) */
        } else {
            const seq_t m_minus1 = 15;   /* 16 - 1 */
            row[i] = 1;          /* T(i,i) */
            for (idx_t k = 1; k <= i - 1; ++k) {
                seq_t a_prev = ((seq_t)(i - 1)) * m_minus1 + (seq_t)k;
                seq_t val = triangle[i - 1][k] * a_prev;
                if (k > 1)
                    val += triangle[i - 1][k - 1];
                row[k] = val;
            }
        }
        triangle[i] = row;
    }
    triangle_rows = r;
}

/* Return the n-th term of OEIS sequence A223518 */
seq_t a(idx_t n)
{
    /* Find the row 'r' that contains the n-th element (1‑based) */
    if (n < 1)
        return (seq_t)0;          /* undefined for n < 1 */

    idx_t r = 1;
    idx_t sum = 1;                 /* cumulative count after current row */
    while (sum < n) {
        ++r;
        sum += r;
    }

    idx_t prev_sum = sum - r;      /* cumulative count before row r */
    idx_t k = n - prev_sum;        /* position within row r (1‑based) */

    ensure_row(r);
    return triangle[r][k];
}
