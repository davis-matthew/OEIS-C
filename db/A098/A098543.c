/*
 * A098543  Column 2 of triangle A098542, which shifts left and up under the
 *          matrix square.
 * https://oeis.org/A098543
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = c(n-1) + c(n-2), where c(m) = Sum_{k=1..m} A002449(k);
 *            i.e. G.f. A(x) = x*(1+x)/(1-x)*(G002449(x)-1)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 11, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 17 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define A002449_MAXR 2048

static seq_t a002449_D[A002449_MAXR + 2];
static seq_t a002449_W[A002449_MAXR + 2];
static seq_t a002449_V[2 * A002449_MAXR + 4];
static seq_t a002449_ans[A002449_MAXR + 1];
static idx_t a002449_rows = 0;
static int a002449_init = 0;

static void a002449_advance(void)
{
    idx_t m = a002449_rows + 1;
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

static seq_t A002449_a(idx_t n)
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

seq_t a(idx_t n)
{
    seq_t c = 0;      /* c(m) running partial sum */
    seq_t cprev = 0;  /* c(m-1) */
    idx_t k;
    if (n < 2) return 0;
    /* want c(n-1) + c(n-2) */
    for (k = 1; k <= n - 1; k++) {
        cprev = c;
        c = c + A002449_a(k);
    }
    /* now c = c(n-1), cprev = c(n-2) */
    return c + cprev;
}
