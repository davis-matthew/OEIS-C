/*
 * A208515  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A208514; see the Formula section.
 * https://oeis.org/A208515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   The sequence is the triangle of coefficients of the polynomials
 *            v(n,x) defined by v(1,x)=1, v(2,x)=x+1, and v(n,x) =
 *            (x+1)*v(n‑1,x) + x*(x-1)*v(n‑2,x) (n ≥ 3). The terms a(n) are
 *            read row‑major from this triangle: row r has r terms and starts
 *            at index T(r-1)+1 where T(k)=k*(k+1)/2
 * Range:     u8 n <= 62, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t **v_rows = NULL;      /* v_rows[r] points to row r coefficients (1‑based) */
static idx_t  max_prepared = 0;    /* largest row computed so far */

/* Allocate/resize the array of row pointers. */
static void ensure_rows(idx_t up_to)
{
    if (up_to <= max_prepared)
        return;

    /* enlarge the pointer array */
    seq_t **new_v = (seq_t**)realloc(v_rows, sizeof(seq_t *) * (up_to + 1));
    if (!new_v)
        abort();
    v_rows = new_v;

    /* initialise the base rows if needed */
    if (max_prepared == 0) {
        /* row 1: v(1,x)=1 */
        v_rows[1] = (seq_t*)malloc(sizeof(seq_t));
        if (!v_rows[1])
            abort();
        v_rows[1][0] = (seq_t)1;
        max_prepared = 1;
    }
    if (up_to >= 2 && max_prepared < 2) {
        /* row 2: v(2,x)=x+1 -> coefficients [1,2] */
        v_rows[2] = (seq_t*)malloc(sizeof(seq_t) * 2);
        if (!v_rows[2])
            abort();
        v_rows[2][0] = (seq_t)1;   /* constant term */
        v_rows[2][1] = (seq_t)2;   /* coefficient of x */
        max_prepared = 2;
    }

    /* compute rows 3 .. up_to */
    for (idx_t n = max_prepared + 1; n <= up_to; ++n) {
        seq_t *prev1 = v_rows[n - 1];    /* v(n-1,x)  length n-1 */
        seq_t *prev2 = v_rows[n - 2];    /* v(n-2,x)  length n-2 */

        seq_t *cur = (seq_t*)malloc(sizeof(seq_t) * n);
        if (!cur)
            abort();

        for (idx_t k = 0; k < n; ++k) {
            seq_t c = 0;

            /* term from v(n-1,x) */
            if (k < n - 1)
                c += prev1[k];
            if (k > 0)
                c += prev1[k - 1];

            /* term from x^2*v(n-2,x) */
            if (k > 1)
                c += prev2[k - 2];

            /* term from –x*v(n-2,x) */
            if (k > 0 && k - 1 < n - 2)
                c -= prev2[k - 1];

            cur[k] = c;
        }

        v_rows[n] = cur;
    }
    max_prepared = up_to;
}

/* Return the n-th term of the sequence (n >= 1) */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Find the row r such that T(r-1) < n <= T(r),  T(r)=r(r+1)/2 */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* Position inside the row (0‑based) */
    idx_t sum_prev = ((r - 1) * r) / 2;
    idx_t pos = n - sum_prev - 1;

    ensure_rows(r);
    return v_rows[r][pos];
}
