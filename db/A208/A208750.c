/*
 * A208750  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A208749; see the Formula section.
 * https://oeis.org/A208750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th entry read row‑by‑row of the triangle T defined
 *            by T[1][1] = 1; T[2][1] = 2; T[2][2] = 1; For i ≥ 3 and 1 ≤ k ≤
 *            i: T[i][k] = 2 · T[i‑1][k] – T[i‑2][k] + 2 · T[i‑2][k‑1] + 2 ·
 *            T[i‑2][k‑2], with the convention that out‑of‑range indices are
 *            0. The sequence a(n) is obtained by listin...
 * Range:     u8 n <= 30, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* static storage for the triangle */
static seq_t **T = NULL;       /* T[i] points to row i (indices 1..i) */
static size_t maxRow = 0;      /* highest row that has been computed */

/* ensure that rows 1..r are available */
static void ensure_rows(size_t r)
{
    if (r <= maxRow)
        return;

    /* grow the array of row pointers if necessary */
    if (T == NULL) {
        T = (seq_t**)calloc(r + 1, sizeof(*T));
    } else {
        T = (seq_t**)realloc(T, (r + 1) * sizeof(*T));
    }

    /* compute new rows i = maxRow+1 .. r */
    for (size_t i = maxRow + 1; i <= r; ++i) {
        /* allocate space for row i (index 1..i) */
        T[i] = (seq_t*)calloc(i + 1, sizeof(*T[i]));
        if (i == 1) {
            T[1][1] = (seq_t)1;
        } else if (i == 2) {
            T[2][1] = (seq_t)2;
            T[2][2] = (seq_t)1;
        } else {
            for (size_t k = 1; k <= i; ++k) {
                seq_t val = 0;

                /* 2 * T[i-1][k] */
                if (k <= i - 1) {
                    val += T[i - 1][k];
                    val += T[i - 1][k];
                }

                /* – T[i-2][k] */
                if (k <= i - 2)
                    val -= T[i - 2][k];

                /* + 2 * T[i-2][k-1] */
                if (k >= 2 && k - 1 <= i - 2) {
                    val += T[i - 2][k - 1];
                    val += T[i - 2][k - 1];
                }

                /* + 2 * T[i-2][k-2] */
                if (k >= 3 && k - 2 <= i - 2) {
                    val += T[i - 2][k - 2];
                    val += T[i - 2][k - 2];
                }

                T[i][k] = val;
            }
        }
    }

    maxRow = r;
}

/* public function defined by the problem statement */
seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;   /* value undefined below the offset */

    /* find the row that contains the n‑th term (rows have lengths 1,2,3,…) */
    idx_t N = n;
    size_t r = 1;
    idx_t cum = 1;           /* cumulative terms up to current row */
    while (cum < N) {
        ++r;
        cum += (idx_t)r;     /* cum = r*(r+1)/2 after the loop */
    }

    /* column index within that row (1‑based) */
    size_t k = (size_t)(N - ((idx_t)(r - 1) * r / 2));

    /* ensure the required rows are computed */
    ensure_rows(r);

    return T[r][k];
}
