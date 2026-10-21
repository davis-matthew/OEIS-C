/*
 * A206800  Riordan array (1/(1-3*x+x^2), x*(1-x)/(1-3*x+x^2)).
 * https://oeis.org/A206800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence definition:
 *
 *  T(0,0) = 1
 *  T(n,k) = 3*T(n-1,k) + T(n-1,k-1) - T(n-2,k) - T(n-2,k-1)
 *  with T(n,k)=0 for k<0 or k>n.
 *
 *  a(n) is the row‑by‑row (left‑to‑right, top‑to‑bottom) enumeration of the
 *  Riordan array T(n,k).  The first few terms are
 *  1, 3, 1, 8, 5, 1, 21, 19, 7, 1, 55, 65, 34, 9, 1, 144, … */

#include <stdlib.h>

static seq_t **triangle = NULL;   /* pointer to each row of the array   */
static idx_t  triangle_size = 0;  /* number of rows currently stored     */

/* Ensure that rows 0 .. r are fully computed in `triangle`. */
static void
ensure_triangle(idx_t r)
{
    while (triangle_size <= r) {
        idx_t i = triangle_size;        /* index of the new row          */
        seq_t *row = (seq_t *)malloc((i + 1) * sizeof(seq_t));
        if (row == NULL) abort();       /* out of memory                 */

        if (i == 0) {                    /* base row 0,0 = 1              */
            row[0] = (seq_t)1;
        } else {
            /* compute each entry of row i using the recurrence            */
            for (idx_t k = 0; k <= i; ++k) {
                seq_t val = 0;

                if (i >= 1) {
                    if (k <= i - 1)                 /* T(i-1,k)   */
                        val += (seq_t)3 * triangle[i - 1][k];
                    if (k > 0 && k - 1 <= i - 1)     /* T(i-1,k-1) */
                        val += triangle[i - 1][k - 1];
                }
                if (i >= 2) {
                    if (k <= i - 2)                 /* T(i-2,k)   */
                        val -= triangle[i - 2][k];
                    if (k > 0 && k - 1 <= i - 2)     /* T(i-2,k-1) */
                        val -= triangle[i - 2][k - 1];
                }
                row[k] = val;
            }
        }

        /* Append the new row to the array of rows.                     */
        seq_t **new_tri = (seq_t **)realloc(triangle,
                                            (triangle_size + 1) * sizeof(seq_t *));
        if (new_tri == NULL) abort();
        triangle = new_tri;
        triangle[triangle_size] = row;
        ++triangle_size;
    }
}

/* Return the nth term of the sequence A206800. */
seq_t a(idx_t n)
{
    /* Find the row r such that the cumulative count of terms up to
       row r is ≤ n < count up to row r+1.  The number of entries
       up to row r is r*(r+1)/2.  Use 128‑bit arithmetic to avoid
       overflow on large indices. */
    idx_t r = 0;
    while (1) {
        __int128 next = (__int128)(r + 1) * (r + 2) / 2;
        if (next > n)
            break;
        ++r;
    }

    /* Build all required rows. */
    ensure_triangle(r);

    /* Position within the found row. */
    __int128 before = (__int128)r * (r + 1) / 2;
    idx_t pos = n - (idx_t)before;

    return triangle[r][pos];
}
