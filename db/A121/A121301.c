/*
 * A121301  Triangle read by rows: T(n,k) is the number of directed
 *          column-convex polyominoes of area n and having k cells in the
 *          shortest column (1<=k<=n).
 * https://oeis.org/A121301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the nth entry of the triangle T(n,k) of directed
 *            column‑convex polyominoes, where a(n) is read by rows. T(n,k) =
 *            number of directed column‑convex polyominoes of area n having a
 *            shortest column of length k. This function implements the
 *            recurrence derived from the rational generating functio...
 * Range:     u8 n <= 28, u16 n <= 78, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Function to compute the coefficents a_m = A(m,k) for given k,
   up to an upper index N.  The result is stored in out[0..N].  */
static void compute_A(idx_t k, idx_t N, seq_t *out)
{
    /* initialise output array */
    for (idx_t i = 0; i <= N; ++i) out[i] = 0;

    /* denominator coefficients d[0..k+1] */
    /* maximum required length is k+2, but k <= 200 is enough
       for any index n <= 10000 because the row number r <= 141 */
    static int d[205];
    for (idx_t i = 0; i <= k + 1; ++i) d[i] = 0;
    d[0] = 1;
    if (k + 1 >= 1) d[1] = -2;
    if (k + 1 >= 2) d[2] = 1;
    d[k]     += -((int)k);
    d[k + 1] += (int)(k - 1);

    /* recurrence to fill a[0..N] */
    for (idx_t m = 0; m <= N; ++m) {
        seq_t temp = 0;

        /* handle the numerator terms n_m */
        if (m == k)          temp += 1;
        else if (m == k + 1) temp -= 1;

        /* add/subtract terms from the denominator */
        for (idx_t i = 1; i <= k + 1 && i <= m; ++i) {
            int coeff = d[i];
            if (coeff < 0) {
                /* coeff negative:  -coeff * a[m-i] */
                temp += (seq_t)(-coeff) * out[m - i];
            } else if (coeff > 0) {
                /* coeff positive:  coeff * a[m-i] */
                temp -= (seq_t)coeff * out[m - i];
            }
        }

        out[m] = temp;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe */

    /* determine the row r and column k for index n */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) ++r;
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t k = n - prev_tri;   /* 1‑based column index */

    /* compute A(r,k) and A(r,k+1) */
    seq_t cur[205];
    seq_t nxt[205];

    compute_A(k, r, cur);
    if (k + 1 <= r)
        compute_A(k + 1, r, nxt);
    else
        for (idx_t i = 0; i <= r; ++i) nxt[i] = 0;

    /* T(r,k) = A(r,k) - A(r,k+1) */
    seq_t result = cur[r];
    result -= nxt[r];
    return result;
}
