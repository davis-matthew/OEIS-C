/*
 * A141021  Pascal-like triangle with index of asymmetry y = 4 and index of
 *          obliqueness z = 1.
 * https://oeis.org/A141021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   triangle G(n,k) with G(n+6,k)=G(n+1,k-4)+G(n+1,k-5)+G(n+2,k-4)+
 *            * G(n+3,k-3)+G(n+4,k-2)+G(n+5,k-1) for n≥0,k=5..n+5, * and
 *            G(n+x+1,x)=2^x for x=0..4 (endpoints G(n,0)=G(n,n)=1). * The
 *            linearized sequence a(n) is obtained by reading the rows of G *
 *            consecutively, starting with G(0,0). * For effici...
 * Range:     u8 n <= 62, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>

/* static storage for the triangular table */
static seq_t **triangle = NULL;      /* pointer to each row array                */
static idx_t  max_row  = (idx_t)-1;   /* largest row already constructed         */
static size_t table_cap = 0;          /* allocated size of 'triangle' array      */

/* helper: compute 2^k as a seq_t (works for any seq_t type) */
static seq_t pow2_idx(idx_t k)
{
    seq_t v = 1;
    for (idx_t i = 0; i < k; ++i)
        v = v * 2;
    return v;
}

/* Ensure that rows 0..r are built and stored in 'triangle'. */
static void ensure_triangle_up_to(idx_t r)
{
    /* grow the pointer array if necessary */
    if (r + 1 > table_cap) {
        size_t new_cap = r + 1;
        if (triangle == NULL) {
            triangle = (seq_t **)malloc(new_cap * sizeof(seq_t *));
        } else {
            seq_t **tmp = (seq_t **)realloc(triangle, new_cap * sizeof(seq_t *));
            if (tmp == NULL) abort();  /* allocation failure */
            triangle = tmp;
        }
        if (triangle == NULL) abort();
        table_cap = new_cap;
    }

    /* build new rows */
    for (idx_t cur = max_row + 1; cur <= r; ++cur) {
        seq_t *row = (seq_t *)malloc((cur + 1) * sizeof(seq_t));
        if (row == NULL) abort();

        for (idx_t k = 0; k <= cur; ++k) {
            /* endpoints */
            if (k == 0 || k == cur) {
                row[k] = 1;
                continue;
            }

            /* base values for k ≤ 4 and row index sufficiently large */
            if (k <= 4) {
                row[k] = pow2_idx(k);      /* 2^k */
                continue;
            }

            /* recurrence for k ≥ 5, row index ≥ 6 */
            if (cur >= 6 && k >= 5 && k <= cur - 1) {
                seq_t s = 0;
                s += triangle[cur - 5][k - 4];
                s += triangle[cur - 5][k - 5];
                s += triangle[cur - 4][k - 4];
                s += triangle[cur - 3][k - 3];
                s += triangle[cur - 2][k - 2];
                s += triangle[cur - 1][k - 1];
                row[k] = s;
            } else {
                /* unreachable for valid indices; safeguard */
                row[k] = 0;
            }
        }

        triangle[cur] = row;
    }
    max_row = r;
}

/* Public interface: compute the nth term of the sequence */
seq_t a(idx_t n)
{
    /* Find the row containing the nth element: r satisfies
       r*(r+1)/2 ≤ n < (r+1)*(r+2)/2   */
    double d  = sqrt((double)(8ULL * n + 1));
    idx_t r   = (idx_t)((d - 1.0) / 2.0);

    /* linear index of the start of this row */
    idx_t offset = r * (r + 1) / 2;
    idx_t k      = n - offset;   /* column offset inside the row */

    ensure_triangle_up_to(r);
    return triangle[r][k];
}
