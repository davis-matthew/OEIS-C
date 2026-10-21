/*
 * A054120  Triangular array T(n,k): start with T(n,0)=T(n,n)=1 for n >= 0;
 *          recursively, draw vertical lines through T(n-1,k-1) if present and
 *          T(n-1,k) if present; then T(n,k) is the sum of T(i,j) that lie on
 *          or between the lines and not below T(n,k).
 * https://oeis.org/A054120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, eigen
 * Formula:   a(n) is the n‑th element of the triangular array T(n,k) defined
 *            by * T(0,0) = 1, * T(1,0) = T(1,1) = 1, * T(2,0) = T(2,2) = 1,
 *            T(2,1) = 3, * for n ≥ 3: T(n,0) = T(n,n) = 1, * and for 1 ≤ k ≤
 *            n‑1: T(n,k) = T(n‑1,k‑1) + 2·T(n‑2,k‑1) + T(n‑1,k). * The
 *            sequence a(n) consists of the elements of T(n,k)...
 * Range:     u8 n <= 30, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t **T = NULL;          /* array of pointers to rows of T */
static idx_t  T_rows = 0;         /* number of rows currently stored */

/* Ensure that rows 0 … r are computed and stored in T. */
static void ensure_rows(idx_t r)
{
    if (T_rows > r)
        return;                    /* already computed up to r */

    /* Grow the array holding row pointers. */
    T = (seq_t **)realloc(T, sizeof(seq_t *) * (r + 1));

    /* Compute new rows. */
    for (idx_t n = T_rows; n <= r; ++n)
    {
        /* Allocate storage for row n. */
        T[n] = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
        if (!T[n]) return;         /* allocation failure – caller will see 0 */

        if (n == 0)
        {
            T[0][0] = (seq_t)1;
        }
        else if (n == 1)
        {
            T[1][0] = (seq_t)1;
            T[1][1] = (seq_t)1;
        }
        else if (n == 2)
        {
            T[2][0] = T[2][2] = (seq_t)1;
            T[2][1] = (seq_t)3;
        }
        else            /* n >= 3 */
        {
            seq_t *prev  = T[n - 1];
            seq_t *prev2 = T[n - 2];
            seq_t *curr  = T[n];

            curr[0] = curr[n] = (seq_t)1;   /* borders */

            for (idx_t k = 1; k < n; ++k)
            {
                curr[k] = prev[k - 1]      /* T(n‑1,k‑1) */
                       + prev[k]            /* T(n‑1,k)   */
                       + (seq_t)2 * prev2[k - 1];  /* 2·T(n‑2,k‑1) */
            }
        }
    }

    T_rows = r + 1;
}

/* Return the a(n) value (row‑major traversal of the triangular array). */
seq_t a(idx_t n)
{
    /* Find the row r that contains the n‑th entry. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column index within row r. */
    idx_t k = n - r * (r + 1) / 2;

    ensure_rows(r);                         /* compute rows up to r if needed */
    return T[r][k];
}
