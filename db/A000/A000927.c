/*
 * A000927  "First factor" (or relative class number) h- for cyclotomic field
 *          Q( exp(2 Pi / prime(n)) ).
 * https://oeis.org/A000927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = h^-(Q(zeta_p)), p = prime(n), computed by the analytic
 *            class number formula h^- = 2p * Prod_{chi odd} (-B_{1,chi}/2),
 *            B_{1,chi} = (1/p) Sum_{a=1..p-1} chi(a)*a (equal to
 *            |det(floor(i*j/p)-floor((i-1)*j/p))|, 3<=i,j<=(p-1)/2); the
 *            product is evaluated modulo many primes q == 1 (mod p-1) and...
 * Range:     u8 n <= 14, u16 n <= 17, u32 n <= 23, u64 n <= 31, u128 n <= 46,
 *            big any n
 * Helpers:   cos_sin, nth_prime_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/cos_sin.h"
#include "../utils/nth_prime_local.h"

typedef unsigned __int128 u128_t;

static uint64_t mulm(uint64_t a, uint64_t b, uint64_t q) { return (uint64_t)((u128_t)a * b % q); }

static uint64_t powm(uint64_t a, uint64_t e, uint64_t q) {
    uint64_t r = 1 % q;
    a %= q;
    while (e) {
        if (e & 1) r = mulm(r, a, q);
        a = mulm(a, a, q);
        e >>= 1;
    }
    return r;
}

static bool isprime64(uint64_t n) {
    static const uint64_t sp[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    if (n < 2) return false;
    for (int i = 0; i < 12; i++) {
        if (n == sp[i]) return true;
        if (n % sp[i] == 0) return false;
    }
    for (uint64_t f = 41; f < 1000 && f * f <= n; f += 2)
        if (n % f == 0) return false;
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (int i = 0; i < 12; i++) {
        uint64_t x = powm(sp[i], d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (int r = 1; r < s; r++) {
            x = mulm(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* Montgomery arithmetic modulo an odd q < 2^62 */
static uint64_t mg_q, mg_ninv;

static void mg_init(uint64_t q) {
    uint64_t x = q;
    for (int i = 0; i < 6; i++) x *= 2 - q * x;
    mg_q = q;
    mg_ninv = (uint64_t)0 - x;
}

static uint64_t mg_redc(u128_t T) {
    uint64_t m = (uint64_t)T * mg_ninv;
    uint64_t t = (uint64_t)((T + (u128_t)m * mg_q) >> 64);
    return t >= mg_q ? t - mg_q : t;
}

static uint64_t mg_to(uint64_t a) { return (uint64_t)(((u128_t)a << 64) % mg_q); }

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;
    uint64_t p = nth_prime_local(n);
    uint64_t N = p - 1, m = N / 2;

    /* prime factors of N */
    uint64_t fac[64];
    int nf = 0;
    {
        uint64_t t = N;
        for (uint64_t d = 2; d * d <= t; d++)
            if (t % d == 0) {
                fac[nf++] = d;
                while (t % d == 0) t /= d;
            }
        if (t > 1) fac[nf++] = t;
    }

    /* primitive root g mod p */
    uint64_t g = 2;
    if (p > 2) {
        for (g = 2;; g++) {
            bool ok = true;
            for (int i = 0; i < nf; i++)
                if (powm(g, N / fac[i], p) == 1) { ok = false; break; }
            if (ok) break;
        }
    } else {
        g = 1;
    }

    /* G(x) = Sum_{j<m} (2 r_j - p) x^j, r_j = g^j mod p */
    long long *cf = (long long *)malloc(sizeof(long long) * (size_t)m);
    {
        uint64_t r = 1;
        for (uint64_t j = 0; j < m; j++) {
            cf[j] = 2 * (long long)r - (long long)p;
            r = r * g % p;
        }
    }

    /* upper bound on log2(h^-) via floating estimate of |G(zeta^k)| */
    long long bits;
    {
        const double PI = 3.14159265358979323846;
        double *ct = (double *)malloc(sizeof(double) * (size_t)N);
        double *st = (double *)malloc(sizeof(double) * (size_t)N);
        for (uint64_t i = 0; i < N; i++) {
            double th = 2.0 * PI * (double)i / (double)N;
            if (th > PI) th -= 2.0 * PI;
            cos_sin(th, &ct[i], &st[i]);
        }
        double M = 1.0;
        long long E = 0;
        double denom = 4.0 * (double)p * (double)p;
        for (uint64_t k = 1; k < N; k += 2) {
            double re = 0.0, im = 0.0;
            uint64_t idx = 0;
            for (uint64_t j = 0; j < m; j++) {
                re += (double)cf[j] * ct[idx];
                im += (double)cf[j] * st[idx];
                idx += k;
                while (idx >= N) idx -= N;
            }
            double v = re * re + im * im;
            M *= 2.0 * (v + 4.0) / denom;
            while (M >= 2.0) { M /= 2.0; E++; }
            while (M < 1.0) { M *= 2.0; E--; }
        }
        free(ct);
        free(st);
        long long hb = E + 1;
        if (hb < 0) hb = 0;
        int bl = 0;
        for (uint64_t t = 2 * p; t; t >>= 1) bl++;
        bits = hb / 2 + 1 + bl + 64;
    }
    int nq = (int)(bits / 61) + 1;

    uint64_t *qs = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)nq);
    uint64_t *rs = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)nq);
    uint64_t *cm = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)m);

    uint64_t t = (((uint64_t)1 << 62) - 1) / N;
    for (int iq = 0; iq < nq; iq++) {
        uint64_t q;
        for (;;) {
            q = t * N + 1;
            t--;
            if (isprime64(q)) break;
        }
        qs[iq] = q;
        /* omega of exact order N */
        uint64_t w = 0;
        for (uint64_t x = 2;; x++) {
            w = powm(x, (q - 1) / N, q);
            bool ok = true;
            for (int i = 0; i < nf; i++)
                if (powm(w, N / fac[i], q) == 1) { ok = false; break; }
            if (ok) break;
        }
        mg_init(q);
        for (uint64_t j = 0; j < m; j++) {
            uint64_t v = cf[j] >= 0 ? (uint64_t)cf[j] % q : q - (uint64_t)(-cf[j]) % q;
            cm[j] = mg_to(v);
        }
        uint64_t wm = mg_to(w);
        uint64_t w2 = mg_redc((u128_t)wm * wm);
        uint64_t wk = wm;
        uint64_t P = mg_to(1);
        for (uint64_t k = 1; k < N; k += 2) {
            uint64_t S = 0;
            for (uint64_t jj = m; jj-- > 0;) {
                S = mg_redc((u128_t)S * wk) + cm[jj];
                if (S >= q) S -= q;
            }
            P = mg_redc((u128_t)P * S);
            wk = mg_redc((u128_t)wk * w2);
        }
        uint64_t h = mg_redc((u128_t)P);
        uint64_t den = powm((2 * p) % q, m - 1, q);
        h = mulm(h, powm(den, q - 2, q), q);
        if (m & 1) h = (q - h) % q;
        rs[iq] = h;
    }

    /* Garner mixed-radix reconstruction */
    uint64_t *cg = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)nq);
    for (int i = 0; i < nq; i++) {
        uint64_t q = qs[i];
        uint64_t val = 0, prod = 1 % q;
        for (int jj = i - 1; jj >= 0; jj--) {
            val = (mulm(val, qs[jj] % q, q) + cg[jj] % q) % q;
        }
        for (int jj = 0; jj < i; jj++) prod = mulm(prod, qs[jj] % q, q);
        uint64_t diff = (rs[i] + q - val) % q;
        cg[i] = mulm(diff, powm(prod, q - 2, q), q);
    }
    seq_t x = (seq_t)cg[nq - 1];
    for (int i = nq - 2; i >= 0; i--) {
        x = x * (seq_t)qs[i] + (seq_t)cg[i];
    }

    free(cf);
    free(qs);
    free(rs);
    free(cm);
    free(cg);
    return x;
}
