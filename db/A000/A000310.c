/*
 * A000310  Coefficients of iterated exponentials.
 * https://oeis.org/A000310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   E.g.f. -log(1+log(1+log(1+log(1-x)))) = f3, where f0 =
 *            -log(1-x), f(i+1) = -log(1-f(i)). With F_i(m) = m![x^m] f_i:
 *            F_0(m) = (m-1)!, F_{i+1}(m) = F_i(m) + Sum_{k=1..m-1}
 *            binomial(m-1,k)*F_i(k)*F_{i+1}(m-k), a(n) = F_3(n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define A310_CAP 20001

static seq_t a310_F[4][A310_CAP];
static seq_t a310_R[A310_CAP];   /* Pascal row: binomial(N-1, k) */
static idx_t a310_N = 0;

seq_t a(idx_t n)
{
    if (n < 1 || n >= A310_CAP)
        return 0;
    while (a310_N < n) {
        idx_t m = a310_N + 1;
        idx_t k;
        int lev;
        /* advance Pascal row to binomial(m-1, .) */
        if (m == 1) {
            a310_R[0] = 1;
        } else {
            a310_R[m - 1] = 1;
            for (k = m - 2; k >= 1; k--)
                a310_R[k] += a310_R[k - 1];
        }
        if (m == 1)
            a310_F[0][1] = 1;
        else
            a310_F[0][m] = a310_F[0][m - 1] * (seq_t)(m - 1);
        for (lev = 1; lev <= 3; lev++) {
            seq_t s = a310_F[lev - 1][m];
            for (k = 1; k < m; k++)
                s += a310_R[k] * a310_F[lev - 1][k] * a310_F[lev][m - k];
            a310_F[lev][m] = s;
        }
        a310_N = m;
    }
    return a310_F[3][n];
}
