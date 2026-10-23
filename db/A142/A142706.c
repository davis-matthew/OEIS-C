/*
 * A142706  Coefficients of the derivatives of the Eulerian polynomials (with
 *          indexing as in A173018).
 * https://oeis.org/A142706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = c*A(r+1,c) where r = ceil((sqrt(8n+1)-1)/2), c = n -
 *            r(r-1)/2, and A(i,j) are Eulerian numbers
 * Range:     u8 n <= 11, u16 n <= 30, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* Static storage for computed Eulerian rows.  euler_rows[n][k] = A(n,k),
   with k = 0..n-1.  The index 0 row is unused. */
static seq_t **euler_rows = NULL;
static idx_t   max_euler  = 0;   /* largest n for which euler_rows[n] is valid */

/* Ensure Eulerian rows 1..n are computed.  Appends rows as needed. */
static void ensure_euler_rows(idx_t n)
{
    if (n <= max_euler)
        return;

    /* Resize the pointer array to hold up to n */
    if (euler_rows == NULL) {
        euler_rows = (seq_t**)malloc(sizeof(seq_t*) * (n + 1));
    } else {
        euler_rows = (seq_t**)realloc(euler_rows, sizeof(seq_t*) * (n + 1));
    }
    if (euler_rows == NULL)
        return;    /* allocation failed – the caller will crash */

    /* Base case: row 1 has a single entry A(1,0) = 1 */
    if (max_euler < 1) {
        euler_rows[1] = (seq_t*)malloc(sizeof(seq_t) * 1);
        euler_rows[1][0] = (seq_t)1;
        max_euler = 1;
    }

    /* Compute successive rows up to n */
    for (idx_t m = max_euler + 1; m <= n; ++m) {
        seq_t *prev = euler_rows[m - 1];
        seq_t *curr = (seq_t*)malloc(sizeof(seq_t) * m);
        for (idx_t k = 0; k < m; ++k) {
            seq_t term1 = 0;
            seq_t term2 = 0;
            if (k > 0) {
                term1 = ((seq_t)(m - k)) * prev[k - 1];
            }
            if (k < m - 1) {
                term2 = ((seq_t)(k + 1)) * prev[k];
            }
            curr[k] = term1 + term2;
        }
        euler_rows[m] = curr;
    }
    max_euler = n;
}

/* Return the n-th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n<1 */

    /* Determine the row r such that r(r+1)/2 >= n */
    idx_t r = (idx_t)((sqrt(8.0 * (double)n + 1.0) - 1.0) * 0.5);
    while (r * (r + 1) / 2 < n)
        ++r;

    /* Column within that row (1-indexed) */
    idx_t col = n - (r * (r - 1) / 2);   /* 1 .. r */

    /* We need Eulerian numbers for n = r+1 */
    ensure_euler_rows(r + 1);

    /* A(r+1, col) is stored at index col in the array */
    seq_t A = euler_rows[r + 1][col];

    /* T(r, col) = col * A(r+1, col) */
    seq_t result = ((seq_t)col) * A;
    return result;
}
