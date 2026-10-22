/*
 * A002577  Number of partitions of 2^n into powers of 2.
 * https://oeis.org/A002577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = t(n,1), a(0) = 1, where t(1,k) = k+1, t(n,0) = 1 and
 *            t(n,k) = t(n,k-1) + t(n-1,2k), i.e. t(n,k) = Sum_{j=0..k}
 *            t(n-1,2j). For fixed n, t(n,k) is a polynomial in k of degree n.
 *            It is kept as its values at k = 0..n and extended to k = 0..2n+2
 *            by finite differences. Those differences are all...
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 13, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#define A002577_MAXL 1024

static seq_t a002577_memo[A002577_MAXL + 2];
static seq_t a002577_V[2 * A002577_MAXL + 8];
static seq_t a002577_D[2 * A002577_MAXL + 8];
static seq_t a002577_E[A002577_MAXL + 8];
static idx_t a002577_lvl = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n > A002577_MAXL) return (seq_t)0;

    if (a002577_lvl == 0) {
        a002577_V[0] = (seq_t)1;
        a002577_V[1] = (seq_t)2;
        a002577_memo[1] = (seq_t)2;
        a002577_lvl = 1;
    }

    while (a002577_lvl < n) {
        long long m = (long long)a002577_lvl;
        long long d = m;
        long long i, j, x, k;
        seq_t s;

        /* backward differences at the last point x = m */
        for (j = 0; j <= m; j++) a002577_D[j] = a002577_V[j];
        a002577_E[0] = a002577_D[m];
        for (i = 1; i <= d; i++) {
            for (j = m; j >= i; j--) a002577_D[j] -= a002577_D[j - 1];
            a002577_E[i] = a002577_D[m];
        }

        /* values of P_m at x = 0..2m+2, stored in D */
        for (x = 0; x <= m; x++) a002577_D[x] = a002577_V[x];
        for (x = m + 1; x <= 2 * m + 2; x++) {
            for (i = d - 1; i >= 0; i--) a002577_E[i] += a002577_E[i + 1];
            a002577_D[x] = a002577_E[0];
        }

        /* P_{m+1}(k) = Sum_{j=0..k} P_m(2j), k = 0..m+1 */
        s = (seq_t)0;
        for (k = 0; k <= m + 1; k++) {
            s += a002577_D[2 * k];
            a002577_V[k] = s;
        }

        a002577_lvl = (idx_t)(m + 1);
        a002577_memo[a002577_lvl] = a002577_V[1];
    }

    return a002577_memo[n];
}
