/*
 * A128899  Riordan array (1,(1-2x-sqrt(1-4x))/(2x)).
 * https://oeis.org/A128899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) uses the Riordan array (1,(1-2x-√(1-4x))/(2x)) to build the
 *            triangle T(n,k) = coeff of xⁿ in (C(x)-1)ᵏ, with C(x) the
 *            Catalan generating function. The terms a(n) are obtained by
 *            reading each triangle row (excluding the leading 0) between
 *            successive triangular numbers. That is, for n > 0: * l...
 * Range:     u8 n <= 28, u16 n <= 67, u32 n <= 210, u64 n <= 667,
 *            u128 n <= 2416, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free */

/* Forward declarations of helper functions */
static void build_triangle_up_to(idx_t m);

static seq_t **triangle = NULL;      /* array of row pointers */
static idx_t triangle_rows = 0;      /* number of computed rows (highest index+1) */

/* Build the Riordan triangle up to and including row m (0‑based). */
static void build_triangle_up_to(idx_t m)
{
    if (triangle_rows > 0 && triangle_rows - 1 >= m) return;   /* already built */

    /* Resize the array of row pointers if necessary */
    size_t new_cap = m + 1;
    if (triangle == NULL) {
        triangle = (seq_t**)malloc(new_cap * sizeof(seq_t*));
    } else {
        triangle = (seq_t**)realloc(triangle, new_cap * sizeof(seq_t*));
    }
    if (!triangle) exit(1);

    /* Build rows from the current highest up to m */
    for (idx_t i = triangle_rows; i <= m; ++i) {
        /* Allocate current row of length i+1 */
        seq_t *row = (seq_t*)malloc((size_t)(i + 1) * sizeof(seq_t));
        if (!row) exit(1);

        /* First entry */
        if (i == 0)
            row[0] = (seq_t)1;
        else
            row[0] = (seq_t)0;                  /* T(n,0) = 0 for n>0 */

        if (i >= 1) {
            seq_t *prev = triangle[i - 1];
            for (idx_t k = 1; k <= i; ++k) {
                seq_t left  = prev[k - 1];
                seq_t mid   = (k <= i - 1) ? prev[k] : (seq_t)0;
                seq_t right = (k + 1 <= i - 1) ? prev[k + 1] : (seq_t)0;
                row[k] = left + (seq_t)2 * mid + right;
            }
        }

        triangle[i] = row;
    }
    triangle_rows = m + 1;
}

/* Return the nth term of the sequence a(n). */
seq_t a(idx_t n)
{
    if (n == 0u)                 /* a(0) = 1 */
        return (seq_t)1;

    /* Find largest m with T_m < n  (T_m = m(m+1)/2) */
    idx_t m = 0u;
    while ( (m + 1u) * (m + 2u) / 2u <= n )
        ++m;

    /* If n itself is a triangular number, the term is zero */
    idx_t triangular = m * (m + 1u) / 2u;
    if (triangular == n)
        return (seq_t)0;

    /* Index inside the block: 0‑based */
    idx_t p = n - triangular - 1u;     /* 0 <= p < m */

    /* Ensure the triangle rows up to m are built */
    build_triangle_up_to(m);

    /* Return the desired entry: T(m, p+1) */
    return triangle[m][p + 1u];
}
