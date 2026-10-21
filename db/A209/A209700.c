/*
 * A209700  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A209699; see the Formula section.
 * https://oeis.org/A209700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th element of the row‑major flattening of the
 *            triangle of coefficients of the polynomials v(n,x) defined by
 *            u(1,x)=1, v(1,x)=1 and u(n,x)=x*u(n-1,x)+x*v(n-1,x)
 *            v(n,x)=2x*u(n-1,x)+v(n-1,x)+1 The entry at row k (k ≥ 1) and
 *            position j (0 ≤ j < k) is the coefficient of x^j in v(k,x).
 *            Th...
 * Range:     u8 n <= 41, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free, exit */

static idx_t max_row_needed = 0;
static seq_t **v_rows = NULL;  /* v_rows[k][j]  coefficient of x^j in v(k,x) */
static seq_t **u_rows = NULL;  /* u_rows[k][j]  coefficient of x^j in u(k,x) */

/* Ensure that rows 1..target are allocated and computed. */
static void
ensure_rows(idx_t target)
{
    if (target <= max_row_needed)
        return;

    /* Grow the pointer arrays to hold rows 1..target */
    v_rows = (seq_t**)realloc(v_rows, sizeof(seq_t*) * (target + 1));
    if (!v_rows) { exit(EXIT_FAILURE); }
    u_rows = (seq_t**)realloc(u_rows, sizeof(seq_t*) * (target + 1));
    if (!u_rows) { exit(EXIT_FAILURE); }

    /* Compute missing rows */
    for (idx_t k = max_row_needed + 1; k <= target; ++k)
    {
        /* Each row has exactly k coefficients for v(k,x) (degree k-1). */
        v_rows[k] = (seq_t*)malloc(sizeof(seq_t) * k);
        u_rows[k] = (seq_t*)malloc(sizeof(seq_t) * k);
        if (!v_rows[k] || !u_rows[k]) { exit(EXIT_FAILURE); }

        if (k == 1)
        {
            /* Initial row: u(1,x)=1, v(1,x)=1 */
            u_rows[1][0] = (seq_t)1;
            v_rows[1][0] = (seq_t)1;
            continue;
        }

        seq_t *u_prev = u_rows[k - 1];
        seq_t *v_prev = v_rows[k - 1];
        seq_t *u_cur  = u_rows[k];
        seq_t *v_cur  = v_rows[k];

        /* Handle the constant term */
        v_cur[0] = v_prev[0] + (seq_t)1;   /* v(k,0) = v(k-1,0) + 1 */

        /* For degrees j >= 1 */
        for (idx_t j = 1; j < k; ++j)
        {
            /* u(k,j) = u(k-1,j-1) + v(k-1,j-1) */
            u_cur[j] = u_prev[j - 1] + v_prev[j - 1];

            /* v(k,j) = 2*u(k-1,j-1) + v(k-1,j)   (v(k-1,j) is zero if j==k-1) */
            seq_t val = v_prev[j];
            if (j > k - 2)   /* j == k-1: v_prev[j] does not exist */
                val = (seq_t)0;
            v_cur[j] = (u_prev[j - 1] * (seq_t)2) + val;
        }

        /* u_cur[0] is implicitly 0 (degree 0 term of u(k,x) is absent) */
        u_cur[0] = (seq_t)0;
    }

    max_row_needed = target;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;  /* undefined for this offset */

    /* Find the row k such that k(k+1)/2 ≥ n. */
    idx_t k = 1;
    while (((k * (k + 1)) / (idx_t)2) < n)
        ++k;

    ensure_rows(k);

    idx_t prev_sum = ((k - 1) * k) / (idx_t)2;   /* total elements before row k */
    idx_t j = n - prev_sum - (idx_t)1;           /* 0‑based position in row k */

    return v_rows[k][j];
}
