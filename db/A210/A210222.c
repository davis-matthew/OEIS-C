/*
 * A210222  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A104698; see the Formula section.
 * https://oeis.org/A210222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the nth element in the triangle formed by the
 * coefficients of v(n,x) defined by
 *   u(1,x)=1, v(1,x)=1
 *   u(n,x)=x*u(n-1,x)+v(n-1,x)+1
 *   v(n,x)=2x*u(n-1,x)+v(n-1,x)+1     (n>1)
 * The triangle is read row-wise from the first coefficient on each
 * row.  This is the sequence in the OEIS entry for A210222.
 */

#include <stdlib.h>
#ifdef __cplusplus
#  include <new>
#endif

struct row
{
    seq_t *u;   /* coefficients of u(n,x) for this row (length = row number) */
    seq_t *v;   /* coefficients of v(n,x) for this row                */
};

static struct row *rows = NULL;   /* array of rows, index 0 → row 1       */
static idx_t      maxrow = 0;     /* number of rows currently stored      */

/* Ensure that rows 1 … target_row are computed and stored. */
static int
ensure_rows(idx_t target_row)
{
    if (target_row <= maxrow)
        return 1;

    struct row *tmp = (struct row *)realloc(rows,
                                            target_row * sizeof(struct row));
    if (!tmp)
        return 0;                       /* allocation failure */
    rows = tmp;

    for (idx_t r = maxrow + 1; r <= target_row; ++r)
    {
        size_t len = (size_t)r;

        seq_t *u_cur = (seq_t *)malloc(len * sizeof(seq_t));
        seq_t *v_cur = (seq_t *)malloc(len * sizeof(seq_t));
        if (!u_cur || !v_cur)
        {
            free(u_cur); free(v_cur);
            return 0;
        }

#ifdef __cplusplus
        for (size_t i = 0; i < len; ++i) new(&u_cur[i]) seq_t();
        for (size_t i = 0; i < len; ++i) new(&v_cur[i]) seq_t();
#endif

        if (r == 1)                     /* base row 1 */
        {
            u_cur[0] = (seq_t)1;
            v_cur[0] = (seq_t)1;
        }
        else
        {
            struct row *prev = &rows[r - 2];
            for (size_t i = 0; i < len; ++i)
            {
                seq_t termUprevShift = (i > 0) ? prev->u[i - 1] : (seq_t)0;
                seq_t termVprev     = (i < len - 1) ? prev->v[i] : (seq_t)0;
                seq_t constCoeff    = (i == 0) ? (seq_t)1 : (seq_t)0;

                u_cur[i] = termUprevShift + termVprev + constCoeff;
                v_cur[i] = (i > 0 ? (termUprevShift * (seq_t)2) : (seq_t)0)
                           + termVprev + constCoeff;
            }
        }

        rows[r - 1].u = u_cur;
        rows[r - 1].v = v_cur;
    }

    maxrow = target_row;
    return 1;
}

/* Return the nth term of the sequence (1‑based index). */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* undefined for n <= 0 */

    /* Find the row r such that r(r+1)/2 ≥ n. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    if (!ensure_rows(r))
        return (seq_t)0;                /* allocation failure */

    idx_t prev_total = ((r - 1) * r) / 2;   /* terms before row r        */
    idx_t pos        = n - prev_total;      /* 1‑based position in row r */
    size_t idx0      = (size_t)(pos - 1);    /* 0‑based array index       */

    return rows[r - 1].v[idx0];
}
