/*
 * A207635  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A207636; see the Formula section.
 * https://oeis.org/A207635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the coefficient of x^pos in the polynomial u(r,x) where
 *            r is the row containing the n‑th term of the triangle of
 *            coefficients produced by: u(1,x) = 1, v(1,x) = 1, u(n,x) =
 *            u(n-1,x) + v(n-1,x), v(n,x) = (x+1)u(n-1,x) + (x+1)v(n-1,x) + 1.
 *            The triangle is read row by row: constant term first...
 * Range:     u8 n <= 23, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Special cases: first two rows contain a single coefficient each. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    /* Find the row r that contains the n-th term.  For r ≥ 3, the cumulative
       number of terms up to row r is (r-1)*r/2 + 1. */
    idx_t r = 3;
    idx_t cum = 4;            /* cumulative terms up to row 3 : 1 + 1 + 2 */
    while (cum < n) {
        r++;
        cum += (r - 1);
    }

    /* Determine the position pos of the term within row r. */
    idx_t pos;
    if (r <= 2) {
        pos = 0;
    } else {
        /* Terms before row r */
        idx_t prev_cum = ((r - 2) * (r - 1)) / 2 + 1;
        pos = n - prev_cum - 1;
    }

    /* Allocate arrays large enough for the largest needed row.  The
       polynomial u(r,x) has r‑1 coefficients for r≥3, and
       v(r,x) has r coefficients. */
    size_t max_len_u = (size_t)(r > 2 ? r - 1 : 1);
    size_t max_len_v = (size_t)(r + 1);  /* one more than max u length */

    seq_t *U  = (seq_t *)calloc(max_len_u, sizeof(seq_t));
    seq_t *V  = (seq_t *)calloc(max_len_v, sizeof(seq_t));
    seq_t *Nu = (seq_t *)calloc(max_len_u, sizeof(seq_t));
    seq_t *Nv = (seq_t *)calloc(max_len_v, sizeof(seq_t));

    /* Initialise row 1: u = v = 1. */
    U[0] = (seq_t)1;
    V[0] = (seq_t)1;
    size_t len_u = 1;
    size_t len_v = 1;

    /* Compute rows 2 .. r.  We keep the current arrays U,V of the
       previous row and compute the arrays Nu,Nv of the next row. */
    for (idx_t cur = 2; cur <= r; ++cur) {
        size_t next_len_u = (cur == 2) ? 1 : (size_t)(cur - 1);
        size_t next_len_v = (size_t)cur;

        /* Compute Nu (u(cur,x)).  */
        for (size_t i = 0; i < next_len_u; ++i) {
            seq_t a_part = (i < len_u) ? U[i] : (seq_t)0;
            seq_t b_part = (i < len_v) ? V[i] : (seq_t)0;
            Nu[i] = a_part + b_part;
        }

        /* Compute Nv (v(cur,x)).  */
        for (size_t i = 0; i < next_len_v; ++i) {
            seq_t sum = 0;
            if (i < len_u)            sum += U[i];
            if (i > 0 && (i - 1) < len_u) sum += U[i - 1];
            if (i < len_v)            sum += V[i];
            if (i > 0 && (i - 1) < len_v) sum += V[i - 1];
            if (i == 0)               sum += 1;      /* +1 constant term */
            Nv[i] = sum;
        }

        /* Swap work arrays for next iteration. */
        size_t tmp_len = len_u;
        len_u = next_len_u;
        len_v = next_len_v;
        seq_t *tmpU = U;  U = Nu;  Nu = tmpU;
        seq_t *tmpV = V;  V = Nv;  Nv = tmpV;
    }

    /* The desired coefficient is now in U[pos]. */
    seq_t result = U[pos];

    /* Free temporary storage. */
    free(U);
    free(V);
    free(Nu);
    free(Nv);

    return result;
}
