/*
 * A002449  Number of different types of binary trees of height n.
 * https://oeis.org/A002449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = T(n,1) where T(0,k) = 1 and T(m,k) = Sum_{j=1..k}
 *            T(m-1,2j) (from F(x,n) = F(x,n-1) + x*F(x,2n), F(x,0) = 1).
 *            T(m,.) is a polynomial in k of degree m, so each row is stored
 *            as forward differences at k = 0 and stepped with additions only
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 13, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define A002449_MAXR 2048

static seq_t a002449_D[A002449_MAXR + 2];      /* forward differences of current row */
static seq_t a002449_W[A002449_MAXR + 2];      /* stepping work array */
static seq_t a002449_V[2 * A002449_MAXR + 4];  /* values of previous row */
static seq_t a002449_ans[A002449_MAXR + 1];
static idx_t a002449_rows = 0;                 /* rows computed through this index */
static int a002449_init = 0;

static void a002449_advance(void)
{
    idx_t m = a002449_rows + 1;   /* build row m from row m-1 (degree m-1, m diffs) */
    idx_t deg = m - 1;
    idx_t i, j, k;

    for (i = 0; i <= deg; i++) a002449_W[i] = a002449_D[i];
    for (j = 0; j <= 2 * m; j++) {
        a002449_V[j] = a002449_W[0];
        for (i = 0; i < deg; i++) a002449_W[i] += a002449_W[i + 1];
    }

    a002449_D[0] = 0;
    for (k = 1; k <= m; k++) a002449_D[k] = a002449_D[k - 1] + a002449_V[2 * k];
    a002449_ans[m] = a002449_D[1];

    for (j = 1; j <= m; j++)
        for (i = m; i >= j; i--)
            a002449_D[i] -= a002449_D[i - 1];

    a002449_rows = m;
}

seq_t a(idx_t n)
{
    if (!a002449_init) {
        a002449_D[0] = 1;
        a002449_ans[0] = 1;
        a002449_rows = 0;
        a002449_init = 1;
    }
    if (n > A002449_MAXR) return 0;
    while (a002449_rows < n) a002449_advance();
    return a002449_ans[n];
}
