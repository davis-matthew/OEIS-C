/*
 * A007650  Number of set-like atomic species of degree n.
 * https://oeis.org/A007650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1, a(n) = Sum_{d|n, d<n} b(d) for n >= 2, where
 *            b = A007649 = Euler transform of a, i.e. Product_{k>=1}
 *            1/(1-x^k)^a(k) = Sum_{n>=0} b(n) x^n. (a is the inverse Euler
 *            transform of A007649.) Computed by a division-safe
 *            Euler-transform DP that builds the product one factor at a time
 * Range:     u8 n <= 23, u16 n <= 49, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   alloc_zero, gcd_seq, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/gcd_seq.h"

static seq_t *A_tab = 0;
static idx_t A_cap = 0; /* A_tab holds a(0..A_cap-1) */

static void build(idx_t N)
{
    seq_t *f = alloc_zero(N + 1);   /* running product coefficients -> b */
    seq_t *acc = alloc_zero(N + 1); /* sum of b(d) over proper divisors d */
    seq_t *na = alloc_zero(N + 1);
    idx_t k, j, m, t;

    f[0] = 1;
    na[0] = 0;
    for (k = 1; k <= N; k++) {
        seq_t ak;
        idx_t J = N / k;
        seq_t *c;

        if (k == 1)
            ak = 1;
        else
            ak = acc[k];
        na[k] = ak;

        /* c[j] = binomial(ak + j - 1, j), coefficients of 1/(1-x^k)^ak */
        c = alloc_zero(J + 1);
        c[0] = 1;
        for (j = 1; j <= J; j++) {
            seq_t g = gcd_seq(c[j - 1], (seq_t)j);
            seq_t jj;
            if (g == 0)
                g = 1;
            jj = (seq_t)j / g;
            c[j] = (c[j - 1] / g) * ((ak + (seq_t)(j - 1)) / jj);
        }

        /* multiply f by 1/(1-x^k)^ak in place, descending m */
        for (m = N; m >= k; m--) {
            seq_t s = 0;
            for (j = 1, t = k; t <= m; j++, t += k)
                s += c[j] * f[m - t];
            f[m] += s;
        }
        release(c, J + 1);

        /* f[k] is now final: b(k) */
        for (m = 2 * k; m <= N; m += k)
            acc[m] += f[k];
    }

    release(f, N + 1);
    release(acc, N + 1);
    if (A_tab)
        release(A_tab, A_cap);
    A_tab = na;
    A_cap = N + 1;
}

seq_t a(idx_t n)
{
    if (n >= A_cap) {
        idx_t N = n;
        if (A_cap > 0 && N < 2 * A_cap)
            N = 2 * A_cap;
        if (N < 64)
            N = 64;
        build(N);
    }
    return A_tab[n];
}
