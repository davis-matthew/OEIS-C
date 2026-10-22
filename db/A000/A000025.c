/*
 * A000025  Coefficients of the 3rd-order mock theta function f(q).
 * https://oeis.org/A000025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   G.f.: (1 + 4 * Sum_{k>=1} (-1)^k * q^(k*(3*k+1)/2) / (1 + q^k))
 *            / Product_{i>=1} (1 - q^i). With c(m) the coefficients of the
 *            numerator, multiplying through by Euler's pentagonal product
 *            gives a(m) = c(m) + Sum_{k>=1} (-1)^(k+1) * (a(m - k*(3*k-1)/2)
 *            + a(m - k*(3*k+1)/2)), a(0) = 1
 * Range:     i8 n <= 32, i16 n <= 109, i32 n <= 384, i64 n <= 1399,
 *            i128 n <= 5258, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *A_tab = 0;
static long long A_len = 0; /* number of computed terms */

static void a_extend(long long n)
{
    long long need = n + 1, m, k, e, i;
    long long *c;
    seq_t *T;
    if (need <= A_len) return;
    if (need < 2 * A_len) need = 2 * A_len;

    c = (long long *)malloc((size_t)need * sizeof(long long));
    for (m = 0; m < need; m++) c[m] = 0;
    c[0] = 1;
    for (k = 1; (e = k * (3 * k + 1) / 2) < need; k++) {
        long long j = 0;
        for (m = e; m < need; m += k, j++)
            c[m] += ((k + j) & 1) ? -4 : 4;
    }

    T = (seq_t *)malloc((size_t)need * sizeof(seq_t));
    for (i = 0; i < A_len; i++) T[i] = A_tab[i];
    for (m = A_len; m < need; m++) {
        seq_t s = 0;
        for (k = 1; ; k++) {
            long long g1 = k * (3 * k - 1) / 2, g2 = g1 + k;
            seq_t t;
            if (g1 > m) break;
            t = T[m - g1];
            if (g2 <= m) t += T[m - g2];
            if (k & 1) s += t; else s -= t;
        }
        s += c[m];
        T[m] = s;
    }
    free(c);
    if (A_tab) free(A_tab);
    A_tab = T;
    A_len = need;
}

seq_t a(idx_t n)
{
    a_extend((long long)n);
    return A_tab[n];
}
