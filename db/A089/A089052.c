/*
 * A089052  Triangle read by rows: T(n,k) (n >= 0, 0 <= k <= n) = number of
 *          partitions of n into exactly k powers of 2.
 * https://oeis.org/A089052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   The OEIS entry defines a triangle T(n,k) of partitions of n into
 *            exactly * k powers of 2 with recurrences * T(2m, k) = T(m, k) +
 *            T(2m-1, k-1) * T(2m+1, k) = T(2m, k-1) * The sequence a(n) is
 *            the triangle read row‑wise, i.e. a(n) is the element * T(r, c)
 *            where r and c satisfy * r*(r+1)/2 <= n < (r...
 * Range:     u8 n <= 5279, u16 n <= 9736, u32 n <= 9736, u64 n <= 9736,
 *            u128 n <= 9736, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static storage for the triangle rows */
static seq_t **triangle = NULL;      /* array of pointers to rows */
static idx_t  triangle_rows = 0;     /* highest row computed  (0‑based) */

/* Grow the triangle up to row r (inclusive). */
static void ensure_triangle_rows(idx_t r)
{
    if (triangle_rows > r)
        return;

    /* grow the array of row pointers if necessary */
    if (triangle == NULL)
        triangle = (seq_t**)calloc(r + 1, sizeof(seq_t*));
    else
        triangle = (seq_t**)realloc(triangle, sizeof(seq_t*) * (r + 1));

    /* compute new rows */
    for (idx_t i = triangle_rows; i <= r; ++i) {
        /* allocate row i with i+1 entries */
        triangle[i] = (seq_t*)calloc(i + 1, sizeof(seq_t));   /* zeroed */

        if (i == 0) {                     /* T(0,0) = 1 */
            triangle[0][0] = (seq_t)1;
            continue;
        }

        triangle[i][0] = (seq_t)0;          /* T(n,0) = 0 for n>0 */

        for (idx_t k = 1; k <= i; ++k) {
            if (i & 1) {                    /* i is odd:  T(2m+1,k) = T(2m,k-1) */
                triangle[i][k] = triangle[i - 1][k - 1];
            } else {                        /* i is even: T(2m,k) = T(m,k)+T(2m-1,k-1) */
                idx_t m = i >> 1;           /* m = i/2  */
                seq_t part1 = (k <= m) ? triangle[m][k] : 0;   /* T(m,k) */
                seq_t part2 = triangle[i - 1][k - 1];          /* T(2m-1,k-1) */
                triangle[i][k] = part1 + part2;
            }
        }
    }

    triangle_rows = r;
}

/* Main sequence function.  n is the global index (0‑based). */
seq_t a(idx_t n)
{
    /* Find the row r that contains the index n. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Ensure the triangle is computed up to row r. */
    ensure_triangle_rows(r);

    /* Index within the row. */
    idx_t start_of_row = r * (r + 1) / 2;      /* start index of row r      */
    idx_t k = n - start_of_row;               /* column inside the row     */

    return triangle[r][k];
}
