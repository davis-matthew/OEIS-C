/*
 * A199400  Triangle T(n,k), read by rows, given by
 *          (2,0,3,0,4,0,5,0,6,0,7,0,8,0,9,...) DELTA
 *          (2,1,3,2,4,3,5,4,6,5,7,6,8,7,9,...) where DELTA is the operator
 *          defined in A084938.
 * https://oeis.org/A199400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (n‑th) entry of triangle T read by rows, where *
 *            T(0,0)=1, * T(n,0)=2^n, * T(n,n)=(n+1)!, * and for 1≤k≤n-1:
 *            T(n,k)=(k+1)*T(n‑1,k‑1)+(k+2)*T(n‑1,k). * The sequence a(n)
 *            lists T(0,0), T(1,0),T(1,1), T(2,0),T(2,1),T(2,2), …
 * Range:     u8 n <= 11, u16 n <= 30, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   factorial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow_uint.h"
#include "../utils/factorial.h"

/* static cache of triangle rows */
static seq_t **rows = NULL;          /* NULL until first use */
static size_t rows_capacity = 0;     /* number of rows currently stored */

/* compute all rows up to and including row r (0‑based) and store them */
static void ensure_row(idx_t r)
{
    if ((size_t)r < rows_capacity)          /* already available */
        return;

    /* grow the pointer array */
    seq_t **tmp = (seq_t **)realloc(rows, sizeof(seq_t *) * ((size_t)r + 1));
    if (!tmp) abort();                      /* allocation failure */
    rows = tmp;

    /* compute new rows */
    for (idx_t i = (idx_t)rows_capacity; i <= r; ++i)
    {
        /* allocate array of length i+1 for row i */
        rows[i] = (seq_t *)calloc((size_t)i + 1, sizeof(seq_t));
        if (!rows[i]) abort();              /* allocation failure */

        if (i == 0)                         /* T(0,0) */
        {
            rows[0][0] = (seq_t)1;
            continue;
        }

        rows[i][0] = pow_uint((uint64_t)2, i);                     /* T(i,0)=2^i */
        rows[i][i] = factorial(i + 1);                            /* T(i,i)=(i+1)! */

        /* recurrence for interior elements */
        for (idx_t k = 1; k < i; ++k)
        {
            seq_t left  = rows[i - 1][k - 1] * (seq_t)(k + 1);
            seq_t right = rows[i - 1][k]     * (seq_t)(k + 2);
            rows[i][k] = left + right;
        }
    }

    rows_capacity = (size_t)r + 1;
}

/* Sequence a(n) */
seq_t a(idx_t n)
{
    /* find row r and column c for linear index n */
    idx_t offset = 0;
    idx_t r = 0;
    while (offset + r + 1 <= n)
    {
        offset += r + 1;
        r++;
    }
    idx_t c = n - offset;      /* column inside row r */

    ensure_row(r);              /* make sure row r is available */
    return rows[r][c];
}
