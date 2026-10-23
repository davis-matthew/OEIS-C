/*
 * A049385  Triangle of numbers related to triangle A049375; generalization of
 *          Stirling numbers of second kind A008277, Lah-numbers A008297...
 * https://oeis.org/A049385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the n-th element (row‑major order) of the triangle
 *            a(n,m) defined by a(1,1) = 1, a(n+1,m) = (5*n + m) * a(n,m) +
 *            a(n,m-1), with a(n,m)=0 when m>n. This implementation builds the
 *            triangle up to the required row on the first call and reuses it
 *            for all subsequent calls. It works for any seq_...
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 36, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t **triangle = NULL;      /* triangle[row][col], 1‑based rows/cols */
static idx_t  max_row = 0;           /* highest row computed so far */

/* Ensure that rows 1..up_to are present */
static void ensure_rows(idx_t up_to)
{
    if (up_to <= max_row) return;

    /* Resize the pointer array */
    triangle = (seq_t**)realloc(triangle,
                                (size_t)(up_to + 1) * sizeof(seq_t*));

    for (idx_t r = max_row + 1; r <= up_to; ++r) {
        /* allocate r+1 elements (indices 0..r) */
        triangle[r] = (seq_t*)malloc((size_t)(r + 1) * sizeof(seq_t));
        triangle[r][0] = 0;               /* a(r,0) = 0 */

        if (r == 1) {                     /* first row */
            triangle[r][1] = 1;           /* a(1,1) = 1 */
            continue;
        }

        seq_t *prev = triangle[r - 1];
        for (idx_t m = 1; m <= r; ++m) {
            seq_t val = 0;
            if (r - 1 >= m) {
                val = prev[m] * (seq_t)(5 * (r - 1) + m);
            }
            val += prev[m - 1];
            triangle[r][m] = val;
        }
    }
    max_row = up_to;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for n<1, return 0 */

    /* Find the row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;

    /* Column m is the position within the row */
    idx_t m = n - ((r - 1) * r) / 2;

    ensure_rows(r);
    return triangle[r][m];
}
