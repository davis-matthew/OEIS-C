/*
 * A156609  Triangle T(n, k, m) = round( t(n,m)/(t(k,m)*t(n-k,m)) ), with T(0,
 *          k, m) = 1, where t(n, k) = Product_{j=1..n} A129862(k+1, j), t(n,
 *          0) = n!, and m = 3, read by rows.
 * https://oeis.org/A156609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = Triangle T(r, k, m) with m = 3, read by rows. T(0, k, m)
 *            = 1, t(n, k) = Π_{j=1..n} A129862(k+1, j), t(n,0)=n!, m = 3. The
 *            values are given by the following explicit rule: T(n,0) = T(n,n)
 *            = 1; T(2,1) = -2; T(n,1) = T(n,n-1) = 4*(-1)^(n+1); T(n,2) =
 *            T(n,n-2) = 8; For interior positions: if n...
 * Range:     i8 n <= 5150, i16 n <= 5150, i32 n <= 5150, i64 n <= 5150,
 *            i128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static seq_t triangle_T(idx_t n, idx_t k)
{
    /* Corners */
    if (k == 0 || k == n)
        return (seq_t)1;

    /* Special case n = 2 */
    if (n == 2 && k == 1)
        return (seq_t)-2;

    /* First off‑diagonal */
    if (k == 1 || k == n - 1) {
        seq_t sign = (((n + 1) & 1) ? (seq_t)-1 : (seq_t)1);
        return 4 * sign;
    }

    /* Second off‑diagonal */
    if (k == 2 || k == n - 2)
        return (seq_t)8;

    /* Interior */
    if ((n & 1) == 0) {                   /* n even */
        seq_t sign = (k & 1) ? (seq_t)-1 : (seq_t)1;
        return 8 * sign;
    } else {                              /* n odd */
        return (seq_t)8;
    }
}

/* Linear index to triangle (row, column) */
static void row_col(idx_t n, idx_t *row, idx_t *col)
{
    /* Pre‑computed triangular numbers up to row 199 */
    static idx_t tri[200];
    static int initialized = 0;

    if (!initialized) {
        initialized = 1;
        for (idx_t i = 0; i < 200; ++i)
            tri[i] = i * (i + 1) / 2;
    }

    /* Find row: largest r with tri[r] <= n */
    idx_t r = 0;
    while (r + 1 < 200 && tri[r + 1] <= n)
        ++r;
    *row = r;
    *col = n - tri[r];
}

seq_t a(idx_t n)
{
    idx_t r, k;
    row_col(n, &r, &k);
    return triangle_T(r, k);
}
