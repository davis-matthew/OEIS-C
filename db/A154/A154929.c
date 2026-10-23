/*
 * A154929  A Fibonacci convolution triangle.
 * https://oeis.org/A154929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the entry T(r,c) of the Fibonacci convolution triangle
 *            with rows 0,1,2,… indexed in lexicographic order, where T(0,0) =
 *            1, T(1,0)=2, T(1,1)=1 and for n ≥ 2, T(n,k) =
 *            T(n-1,k)+T(n-1,k-1)+T(n-2,k)+T(n-2,k-1), with T(n,k)=0 when k>n.
 *            Then a(n) = T(r,c) where r(r+1)/2 ≤ n < (r+1)(r+2)/2 and c...
 * Range:     u8 n <= 29, u16 n <= 94, u32 n <= 307, u64 n <= 1142,
 *            u128 n <= 4217, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Forward declaration of helper functions. */
static idx_t find_row(idx_t n);
static void ensure_rows(idx_t upto);

/* Storage for the triangle rows.  tri[i] points to the i‑th row,
   which contains i+1 entries. */
static seq_t **tri = NULL;   /* pointer to rows of the triangle   */
static idx_t  tri_rows = 0;  /* number of rows currently computed */
static size_t tri_cap  = 0;   /* allocated capacity for row pointers */

/* Return the row index r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2,
   i.e. the smallest r with (r+1)(r+2)/2 > n. */
static inline idx_t find_row(idx_t n)
{
    idx_t r = 0;
    /* Use 128‑bit arithmetic to avoid overflow in the test. */
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n) ++r;
    return r;
}

/* Ensure that rows 0 … upto inclusive are computed and stored in tri. */
static void ensure_rows(idx_t upto)
{
    if (tri_rows > upto) return;

    /* Grow the array of row pointers if necessary. */
    size_t new_cap = tri_cap;
    while (new_cap <= (size_t)upto) new_cap = new_cap ? 2 * new_cap : 8;

    if (new_cap != tri_cap) {
        seq_t **tmp = (seq_t**)realloc(tri, new_cap * sizeof(seq_t*));
        if (!tmp) abort();
        tri = tmp;
        tri_cap = new_cap;
    }

    for (idx_t i = tri_rows; i <= upto; ++i) {
        seq_t *row = (seq_t*)malloc((i + 1) * sizeof(seq_t));
        if (!row) abort();

        if (i == 0) {
            row[0] = 1;
        } else if (i == 1) {
            row[0] = 2;
            row[1] = 1;
        } else {
            seq_t *prev  = tri[i - 1];
            seq_t *prev2 = tri[i - 2];

            /* k = 0 */
            row[0] = prev[0] + prev2[0];

            for (idx_t k = 1; k <= i; ++k) {
                seq_t val = 0;
                if (k <= i - 1)   val += prev[k];
                if (k - 1 <= i - 1) val += prev[k - 1];
                if (k <= i - 2)   val += prev2[k];
                if (k - 1 <= i - 2) val += prev2[k - 1];
                row[k] = val;
            }
        }
        tri[i] = row;
    }

    tri_rows = upto + 1;
}

/* Return the n-th term of A154929. */
seq_t a(idx_t n)
{
    idx_t r = find_row(n);

    /* Ensure the needed rows have been computed. */
    if (tri_rows <= r) ensure_rows(r);

    /* Compute column index within the identified row. */
    idx_t prefix = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t c = n - prefix;

    return tri[r][c];
}
