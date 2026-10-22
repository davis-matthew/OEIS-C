/*
 * A003426  Number of stable trees with n nodes.
 * https://oeis.org/A003426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000055(n) - A002988(n) + [n <= 2], where A002988(n)
 *            counts trees with no leaf adjacent to a node of degree 2. Both
 *            counts are taken over centroids: planted branches satisfy R(s) =
 *            Wr(s-1) and F(s) = W(s-1) - [s=2], where W and Wr are Euler
 *            transforms (multisets of branches). Then t(n) = #...
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 28, u64 n <= 50, u128 n <= 92,
 *            big any n
 * Helpers:   a3426_fixed, a3426_gcd, a3426_half
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/a3426_fixed.h"
#include "../utils/a3426_gcd.h"
#include "../utils/a3426_half.h"

#define A3426_MAXN 10001

static seq_t a3426_G[A3426_MAXN], a3426_Gr[A3426_MAXN], a3426_coef[A3426_MAXN];
static seq_t a3426_BF[A3426_MAXN], a3426_BW[A3426_MAXN], a3426_BR[A3426_MAXN];
static seq_t a3426_BWr[A3426_MAXN], a3426_Bc[A3426_MAXN], a3426_Bcr[A3426_MAXN];
static idx_t a3426_bnext = 0;

/* multiply series G[0..len-1] by (1-x^k)^(-m) */
static void a3426_mul(seq_t *G, idx_t len, idx_t k, seq_t m)
{
    idx_t jmax = (len - 1) / k, j, s;
    a3426_coef[0] = 1;
    for (j = 1; j <= jmax; j++) {
        seq_t num = m + (seq_t)(j - 1);
        idx_t r = (idx_t)(long long)(num % (seq_t)j);
        idx_t g = a3426_gcd(j, r);
        a3426_coef[j] = (a3426_coef[j - 1] / (seq_t)(j / g)) * (num / (seq_t)g);
    }
    for (s = len - 1; s >= k; s--) {
        seq_t acc = G[s];
        for (j = 1; j * k <= s; j++) acc += a3426_coef[j] * G[s - j * k];
        G[s] = acc;
    }
}

static seq_t a3426_fixed_calc(idx_t n)
{
    idx_t K = (n - 1) / 2, k, s;
    seq_t t, b;
    for (s = 0; s < n; s++) { a3426_G[s] = 0; a3426_Gr[s] = 0; }
    a3426_G[0] = 1; a3426_Gr[0] = 1;
    for (k = 1; k <= K; k++) {
        seq_t Fk = a3426_G[k - 1] - (seq_t)(k == 2 ? 1 : 0);
        seq_t Rk = a3426_Gr[k - 1];
        a3426_mul(a3426_G, n, k, Fk);
        a3426_mul(a3426_Gr, n, k, Rk);
    }
    t = a3426_Gr[n - 1];
    b = a3426_G[n - 1];
    if (n == 3) b -= 1;
    if (n % 2 == 0) {
        idx_t h = n / 2;
        seq_t Fh = a3426_G[h - 1] - (seq_t)(h == 2 ? 1 : 0);
        seq_t Rh = a3426_Gr[h - 1];
        t += a3426_half(Rh);
        b += a3426_half(Fh);
    }
    return t - b;
}

static void a3426_extend(idx_t N)
{
    if (a3426_bnext == 0) {
        a3426_BW[0] = 1; a3426_BWr[0] = 1;
        a3426_bnext = 1;
    }
    while (a3426_bnext <= N) {
        idx_t s = a3426_bnext, i;
        seq_t sum = 0, sumr = 0;
        a3426_BF[s] = a3426_BW[s - 1] - (seq_t)(s == 2 ? 1 : 0);
        a3426_BR[s] = a3426_BWr[s - 1];
        {
            seq_t cf = a3426_BF[s] * (seq_t)s;
            seq_t cr = a3426_BR[s] * (seq_t)s;
            for (i = s; i < A3426_MAXN; i += s) {
                a3426_Bc[i] += cf;
                a3426_Bcr[i] += cr;
            }
        }
        for (i = 1; i <= s; i++) {
            sum += a3426_Bc[i] * a3426_BW[s - i];
            sumr += a3426_Bcr[i] * a3426_BWr[s - i];
        }
        a3426_BW[s] = sum / (seq_t)s;
        a3426_BWr[s] = sumr / (seq_t)s;
        a3426_bnext++;
    }
}

static seq_t a3426_big_calc(idx_t n)
{
    idx_t m, lo = (n + 1) / 2;
    seq_t t, b, st = 0, sb = 0;
    a3426_extend(n - 1);
    for (m = lo; m <= n - 1; m++) {
        st += a3426_BR[m] * a3426_BWr[n - 1 - m];
        sb += a3426_BF[m] * a3426_BW[n - 1 - m];
    }
    t = a3426_BWr[n - 1] - st;
    b = a3426_BW[n - 1] - sb;
    if (n == 3) b -= 1;
    if (n % 2 == 0) {
        idx_t h = n / 2;
        t += a3426_half(a3426_BR[h]);
        b += a3426_half(a3426_BF[h]);
    }
    return t - b;
}

seq_t a(idx_t n)
{
    if (n <= 2) return 1;
    if (n >= A3426_MAXN) return 0;
    if (a3426_fixed()) {
        if (n > 1000) return 0;
        return a3426_fixed_calc(n);
    }
    return a3426_big_calc(n);
}
