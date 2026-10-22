/*
 * A000187  Generalized Euler numbers, c(5,n).
 * https://oeis.org/A000187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   E.g.f. A(x) = 2*cos(x)*cos(3*x)/cos(5*x) =
 *            (cos(2*x)+cos(4*x))/cos(5*x), hence a(n) = (-1)^n*(4^n + 16^n) +
 *            Sum_{k=1..n} (-1)^(k+1)*binomial(2n,2k)*25^k*a(n-k), a(0) = 2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define A187_MAXN 16384

static seq_t a187_A[A187_MAXN + 1];
static seq_t a187_row[2 * A187_MAXN + 3]; /* Pascal row 2*(cnt-1) */
static seq_t a187_p4, a187_p16;           /* 4^(cnt-1), 16^(cnt-1) */
static idx_t a187_cnt = 0;
static idx_t a187_r = 0;

static void a187_pascal_step(void)
{
    idx_t j;
    a187_r++;
    a187_row[a187_r] = 0;
    for (j = a187_r; j >= 1; j--)
        a187_row[j] += a187_row[j - 1];
}

seq_t a(idx_t n)
{
    while (a187_cnt <= n) {
        idx_t m = a187_cnt, k;
        seq_t pos, neg, p, s;
        if (m == 0) {
            a187_row[0] = 1;
            a187_r = 0;
            a187_p4 = 1;
            a187_p16 = 1;
        } else {
            a187_pascal_step();
            a187_pascal_step();
            a187_p4 *= 4;
            a187_p16 *= 16;
        }
        pos = 0;
        neg = 0;
        p = 1;
        for (k = 1; k <= m; k++) {
            seq_t term;
            p *= 25;
            term = a187_row[2 * k] * p;
            term *= a187_A[m - k];
            if (k & 1)
                pos += term;
            else
                neg += term;
        }
        s = a187_p4 + a187_p16;
        if (m & 1)
            neg += s;
        else
            pos += s;
        a187_A[m] = pos - neg;
        a187_cnt++;
    }
    return a187_A[n];
}
