/*
 * A132062  Sheffer triangle (1,1-sqrt(1-2*x)). Extended Bessel triangle
 *          A001497.
 * https://oeis.org/A132062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the linearized entry of the Sheffer triangle with
 *            generating function 1/(1−√(1−2x)). Recurrence for triangle
 *            T(n,m): T(0,0)=1 T(n,0)=0 for n>0
 *            T(n,m)=(2*(n-1)-m)*T(n-1,m)+T(n-1,m-1) (1≤m≤n) The sequence a(n)
 *            lists the triangle rows in order: a(0)=T(0,0), a(1)=T(1,0),
 *            a(2)=T(1,1), a(3)=T(2...
 * Range:     u8 n <= 21, u16 n <= 36, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <new>
#else
#include <stdlib.h>
#endif

seq_t a(idx_t n)
{
    /* Find the row r containing the n-th entry */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;

    /* Index within the row */
    idx_t offset = r * (r + 1) / 2;
    idx_t c = n - offset;          /* column index */

    /* Static storage for already computed rows */
    static seq_t **tri = NULL;      /* array of pointers to rows */
    static size_t  tri_size = 0;    /* number of rows stored */

    /* Grow the array to contain row r */
    while (tri_size <= r)
    {
        /* Allocate the current row */
        idx_t len = tri_size + 1;
        seq_t *row;
#ifdef __cplusplus
        row = new seq_t[len];
#else
        row = (seq_t*)malloc(sizeof(seq_t) * (size_t)len);
#endif
        if (row == NULL) abort();    /* out of memory */

        /* Build the row using the recurrence */
        if (tri_size == 0)    /* first row: T(0,0)=1 */
        {
            row[0] = 1;
        }
        else
        {
            row[0] = 0;      /* T(n,0)=0 for n>0 */
            for (idx_t m = 1; m <= tri_size; ++m)
            {
                seq_t term1 = tri[tri_size - 1][m - 1];   /* T(n-1,m-1) */
                seq_t term2 = 0;
                if (m <= tri_size - 1)                     /* m ≤ n-1 */
                {
                    seq_t coeff = (seq_t)((2 * (tri_size - 1)) - m);
                    term2 = coeff * tri[tri_size - 1][m];
                }
                row[m] = term1 + term2;
            }
        }

        /* Store the row */
        tri = (seq_t**)realloc(tri, sizeof(seq_t*) * (tri_size + 1));
        if (tri == NULL) abort();
        tri[tri_size] = row;
        ++tri_size;
    }

    /* Return the requested entry */
    return tri[r][c];
}
