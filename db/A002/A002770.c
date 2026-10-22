/*
 * A002770  Integers connected with coefficients in expansion of Weierstrass
 *          P-function.
 * https://oeis.org/A002770
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   a(n) = H(4n) - 1/2 - Sum_{p prime, p == 1 (mod 4), (p-1) | 4n}
 *            chi(p)^(4n/(p-1))/p, where H(4n) = 4n*(4n-2)!*c(n)/2^(4n) are
 *            the Hurwitz numbers, from the Weierstrass function P(z) = 1/z^2
 *            + Sum_{n>=1} c(n) z^(4n-2) (g2 = 4, g3 = 0): c(1) = 1/5, c(n) =
 *            3*Sum_{j=1..n-1} c(j)*c(n-j) / ((4n+1)*(2n-3...
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 6, i64 n <= 9, i128 n <= 14,
 *            big any n
 * Helpers:   a2770_isprime, a2770_isq, a2770_pw
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/a2770_isprime.h"
#include "../utils/a2770_isq.h"
#include "../utils/a2770_pw.h"

static uint64_t *a2770_Q = NULL;
static size_t a2770_nQ = 0, a2770_capQ = 0;

static void a2770_need_primes(size_t k)
{
    uint64_t c;
    if (a2770_nQ >= k) return;
    if (a2770_capQ < k) {
        size_t nc = a2770_capQ ? a2770_capQ : 64;
        while (nc < k) nc *= 2;
        a2770_Q = (uint64_t *)realloc(a2770_Q, nc * sizeof(uint64_t));
        a2770_capQ = nc;
    }
    c = a2770_nQ ? a2770_Q[a2770_nQ - 1] - 2 : 2147483647ULL;
    while (a2770_nQ < k) {
        if (a2770_isprime(c)) a2770_Q[a2770_nQ++] = c;
        c -= 2;
    }
}

seq_t a(idx_t n)
{
    uint64_t N4 = 4 * (uint64_t)n;
    long long B = 64;
    uint64_t k, d;
    size_t K, i, j;
    uint64_t *ps, *es, *res, *c;
    long long *chis, *dg;
    size_t np = 0, last;
    seq_t r;

    /* bound on bits of |a(n)|: |H(4n)| ~ 4*(4n)!/(2*pi_lemn)^(4n) */
    for (k = 2; k <= N4; k++) {
        uint64_t t = k;
        while (t) { B++; t >>= 1; }
    }
    B -= (long long)(2 * N4);
    if (B < 64) B = 64;
    K = (size_t)(B / 30) + 2;
    a2770_need_primes(K);

    /* primes p == 1 (mod 4) with (p-1) | 4n */
    ps = (uint64_t *)malloc(64 * sizeof(uint64_t));
    es = (uint64_t *)malloc(64 * sizeof(uint64_t));
    chis = (long long *)malloc(64 * sizeof(long long));
    {
        size_t cap = 64;
        for (d = 1; d * d <= N4; d++) {
            uint64_t dd[2];
            int nd, t;
            if (N4 % d) continue;
            dd[0] = d;
            dd[1] = N4 / d;
            nd = (dd[1] == d) ? 1 : 2;
            for (t = 0; t < nd; t++) {
                uint64_t e = dd[t], p = e + 1, x;
                if (e % 4 || !a2770_isprime(p)) continue;
                for (x = 1; x * x < p; x += 2) {
                    uint64_t rem = p - x * x, y = a2770_isq(rem);
                    if (y * y == rem) {
                        long long xs;
                        if (y % 4 == 0) xs = (x % 4 == 1) ? (long long)x : -(long long)x;
                        else xs = (x % 4 == 3) ? (long long)x : -(long long)x;
                        if (np == cap) {
                            cap *= 2;
                            ps = (uint64_t *)realloc(ps, cap * sizeof(uint64_t));
                            es = (uint64_t *)realloc(es, cap * sizeof(uint64_t));
                            chis = (long long *)realloc(chis, cap * sizeof(long long));
                        }
                        ps[np] = p;
                        es[np] = N4 / e;
                        chis[np] = 2 * xs;
                        np++;
                        break;
                    }
                }
            }
        }
    }

    res = (uint64_t *)malloc(K * sizeof(uint64_t));
    c = (uint64_t *)malloc(((size_t)n + 1) * sizeof(uint64_t));
    for (i = 0; i < K; i++) {
        uint64_t q = a2770_Q[i], Q2 = q * q, m, H, f, v;
        c[1] = a2770_pw(5, q - 2, q);
        for (m = 2; m <= n; m++) {
            uint64_t acc = 0, den, jj;
            for (jj = 1; 2 * jj < m; jj++) {
                acc += c[jj] * c[m - jj];
                if (acc >= Q2) acc -= Q2;
            }
            acc %= q;
            acc = 2 * acc % q;
            if (m % 2 == 0) acc = (acc + c[m / 2] * c[m / 2]) % q;
            den = (4 * m + 1) % q * ((2 * m - 3) % q) % q;
            c[m] = 3 * acc % q * a2770_pw(den, q - 2, q) % q;
        }
        f = 1;
        for (k = 2; k + 2 <= N4; k++) f = f * (k % q) % q;
        H = c[n] * (N4 % q) % q * f % q;
        H = H * a2770_pw(a2770_pw(2, N4, q), q - 2, q) % q;
        v = (H + q - a2770_pw(2, q - 2, q)) % q;
        for (j = 0; j < np; j++) {
            uint64_t ch = chis[j] >= 0 ? (uint64_t)chis[j] % q : q - ((uint64_t)(-chis[j]) % q);
            uint64_t term = a2770_pw(ch, es[j], q) * a2770_pw(ps[j] % q, q - 2, q) % q;
            v = (v + q - term) % q;
        }
        res[i] = v;
    }

    /* balanced mixed-radix (Garner) digits */
    dg = (long long *)malloc(K * sizeof(long long));
    for (i = 0; i < K; i++) {
        uint64_t q = a2770_Q[i], t = 0, P = 1, dd;
        for (j = 0; j < i; j++) {
            uint64_t djm = dg[j] >= 0 ? (uint64_t)dg[j] : q - (uint64_t)(-dg[j]);
            t = (t + djm % q * P) % q;
            P = P * (a2770_Q[j] % q) % q;
        }
        dd = (res[i] + q - t) % q * a2770_pw(P, q - 2, q) % q;
        dg[i] = dd > q / 2 ? (long long)dd - (long long)q : (long long)dd;
    }

    r = 0;
    last = K;
    for (i = K; i-- > 0;)
        if (dg[i] != 0) { last = i; break; }
    if (last < K) {
        for (i = last + 1; i-- > 0;) {
            r = r * (idx_t)a2770_Q[i];
            if (dg[i] >= 0) r += (idx_t)dg[i];
            else r -= (idx_t)(-dg[i]);
        }
    }

    free(ps); free(es); free(chis); free(res); free(c); free(dg);
    return r;
}
