/*
 * A002576  Coefficients of Bell's formula for making change.
 * https://oeis.org/A002576
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002577(n) - 2*A002577(n-1), where A002577(n) = number of
 *            partitions of 2^n into powers of 2. A002577(n) = c_n(1), with
 *            c_0(t) = 1 and c_j(t) = Sum_{i=0..t} c_{j-1}(2i) (c_j is a
 *            polynomial of degree j in t)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define A2576_MAXN 2048

static seq_t a2576_memo[A2576_MAXN + 2];  /* memo[j] = A002577(j) */
static seq_t a2576_cur[A2576_MAXN + 2];   /* values c_lvl(0..lvl) */
static seq_t a2576_D[A2576_MAXN + 2];
static idx_t a2576_lvl = 0;
static int a2576_init = 0;

static void a2576_advance(void)
{
    idx_t m = a2576_lvl;      /* c_m: degree m, values at 0..m */
    idx_t j = m + 1;
    idx_t k, i, t;
    seq_t s;

    for (k = 0; k <= m; k++)
        a2576_D[k] = a2576_cur[k];
    /* convert to forward differences at 0 (all intermediates are nonnegative) */
    for (k = 1; k <= m; k++)
        for (i = m; i >= k; i--)
            a2576_D[i] -= a2576_D[i - 1];

    s = 0;
    for (t = 0; t <= 2 * j; t++) {
        if (t % 2 == 0) {
            s += a2576_D[0];
            a2576_cur[t / 2] = s;
        }
        for (k = 0; k < m; k++)
            a2576_D[k] += a2576_D[k + 1];
    }
    a2576_lvl = j;
    a2576_memo[j] = a2576_cur[1];
}

seq_t a(idx_t n)
{
    if (!a2576_init) {
        a2576_cur[0] = 1;
        a2576_memo[0] = 1;
        a2576_lvl = 0;
        a2576_init = 1;
    }
    if (n < 1 || n > A2576_MAXN)
        return 0;
    while (a2576_lvl < n)
        a2576_advance();
    return a2576_memo[n] - a2576_memo[n - 1] * 2;
}
