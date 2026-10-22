/*
 * A006311  Coefficients of period polynomials.
 * https://oeis.org/A006311
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [x^1] f(p,x), p = A000040(n), where f(p,x) =
 *            Product_{j=0..p-1} (x - eta_j) is the period polynomial of the
 *            degree-p subfield of Q(zeta_{p^2}): eta_j = Sum_{h^(p-1) == 1
 *            (mod p^2)} zeta^((1+j*p)*h). Hence a(n) = Sum_i Product_{j<>i}
 *            eta_j; computed exactly modulo many primes q == 1 (mod p^...
 * Range:     i8 n <= 4, i16 n <= 5, i32 n <= 8, i64 n <= 11, i128 n <= 17,
 *            big any n
 * Helpers:   from_i128, nth_prime, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_is_big.h"
#include "../utils/from_i128.h"
#include "../utils/nth_prime.h"

typedef unsigned __int128 u128;

static uint64_t mm(uint64_t a, uint64_t b, uint64_t q) { return (uint64_t)((u128)a * b % q); }

static uint64_t pm(uint64_t b, uint64_t e, uint64_t q)
{
    uint64_t r = 1 % q;
    b %= q;
    while (e) {
        if (e & 1) r = mm(r, b, q);
        b = mm(b, b, q);
        e >>= 1;
    }
    return r;
}

static bool mr(uint64_t n)
{
    static const uint64_t bs[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    int i, r, s = 0;
    uint64_t d = n - 1;
    if (n < 2) return false;
    for (i = 0; i < 12; i++)
        if (n % bs[i] == 0) return n == bs[i];
    while (!(d & 1)) { d >>= 1; s++; }
    for (i = 0; i < 12; i++) {
        uint64_t x = pm(bs[i], d, n);
        if (x == 1 || x == n - 1) continue;
        for (r = 1; r < s; r++) {
            x = mm(x, x, n);
            if (x == n - 1) break;
        }
        if (r == s) return false;
    }
    return true;
}

/* Montgomery arithmetic modulo MQ < 2^63 */
static uint64_t MQ, MQI, MR2;

static void mont_set(uint64_t q)
{
    uint64_t x = q, r1;
    int i;
    for (i = 0; i < 5; i++) x *= 2 - q * x;
    MQ = q;
    MQI = (uint64_t)0 - x;
    r1 = (uint64_t)((((u128)1) << 64) % q);
    MR2 = (uint64_t)((u128)r1 * r1 % q);
}

static uint64_t mmul(uint64_t a, uint64_t b)
{
    u128 T = (u128)a * b;
    uint64_t m = (uint64_t)T * MQI;
    uint64_t t = (uint64_t)((T + (u128)m * MQ) >> 64);
    return t >= MQ ? t - MQ : t;
}

static uint64_t to_m(uint64_t x) { return mmul(x % MQ, MR2); }

seq_t a(idx_t n)
{
    uint64_t p = nth_prime(n), P2 = p * p;
    uint64_t t, x, j, k;
    int bl = 0;
    size_t i, L, jj;
    for (t = p; t; t >>= 1) bl++;
    {
        uint64_t bits = ((p + 1) / 2) * (uint64_t)bl + 2;
        L = (size_t)(bits / 61 + 1);
    }
    uint64_t *hq = (uint64_t *)malloc(p * sizeof(uint64_t));
    uint64_t *Z0 = (uint64_t *)malloc(p * sizeof(uint64_t));
    uint64_t *W = (uint64_t *)malloc(p * sizeof(uint64_t));
    uint64_t *A = (uint64_t *)malloc(p * sizeof(uint64_t));
    uint64_t *eta = (uint64_t *)malloc(p * sizeof(uint64_t));
    uint64_t *pre = (uint64_t *)malloc((p + 1) * sizeof(uint64_t));
    uint64_t *qs = (uint64_t *)malloc(L * sizeof(uint64_t));
    uint64_t *rs = (uint64_t *)malloc(L * sizeof(uint64_t));
    long long *d = (long long *)malloc(L * sizeof(long long));
    uint64_t *cur = (uint64_t *)malloc(L * sizeof(uint64_t));
    uint64_t *Qm = (uint64_t *)malloc(L * sizeof(uint64_t));
    seq_t result;

    /* h_x = x^p mod p^2 is the Teichmueller lift of x; h_x mod p == x */
    for (x = 1; x < p; x++) hq[x] = pm(x, p, P2) / p;

    k = (((uint64_t)1) << 62) / P2;
    for (i = 0; i < L; i++) {
        uint64_t q, z = 0, w = 0, g, one, zm, wm, suf, res;
        do { q = 1 + k * P2; k--; } while (!mr(q));
        for (g = 2;; g++) {
            z = pm(g, (q - 1) / P2, q);
            w = pm(z, p, q);
            if (w != 1) break;
        }
        mont_set(q);
        one = to_m(1); zm = to_m(z); wm = to_m(w);
        Z0[0] = one; W[0] = one;
        for (t = 1; t < p; t++) {
            Z0[t] = mmul(Z0[t - 1], zm);
            W[t] = mmul(W[t - 1], wm);
        }
        /* A[x] = zeta^(h_x) = w^(h_x / p) * zeta^(h_x mod p) */
        for (x = 1; x < p; x++) A[x] = mmul(W[hq[x]], Z0[x]);
        /* eta_j = Sum_x zeta^(h_x) * w^(j*x) */
        for (j = 0; j < p; j++) {
            uint64_t s = 0, idx = 0;
            for (x = 1; x < p; x++) {
                idx += j;
                if (idx >= p) idx -= p;
                s += mmul(A[x], W[idx]);
                if (s >= q) s -= q;
            }
            eta[j] = s;
        }
        pre[0] = one;
        for (j = 0; j < p; j++) pre[j + 1] = mmul(pre[j], eta[j]);
        suf = one; res = 0;
        for (j = p; j-- > 0;) {
            res += mmul(pre[j], suf);
            if (res >= q) res -= q;
            suf = mmul(suf, eta[j]);
        }
        qs[i] = q;
        rs[i] = mmul(res, 1);
    }

    /* Garner's algorithm with balanced digits */
    for (i = 0; i < L; i++) { cur[i] = 0; Qm[i] = 1; }
    for (i = 0; i < L; i++) {
        uint64_t qi = qs[i];
        uint64_t diff = (rs[i] + qi - cur[i]) % qi;
        uint64_t di = mm(diff, pm(Qm[i], qi - 2, qi), qi);
        long long ds = di > qi / 2 ? (long long)di - (long long)qi : (long long)di;
        d[i] = ds;
        for (jj = i + 1; jj < L; jj++) {
            uint64_t qj = qs[jj];
            uint64_t dm = ds >= 0 ? (uint64_t)ds % qj
                                  : (qj - ((uint64_t)(-ds)) % qj) % qj;
            cur[jj] = (cur[jj] + mm(dm, Qm[jj], qj)) % qj;
            Qm[jj] = mm(Qm[jj], qi % qj, qj);
        }
    }

    if (seq_is_big()) {
        seq_t X = 0, Qs = 1;
        for (i = 0; i < L; i++) {
            X += (seq_t)d[i] * Qs;
            Qs *= (seq_t)(long long)qs[i];
        }
        result = X;
    } else {
        u128 acc = 0, Qa = 1;
        for (i = 0; i < L; i++) {
            acc += (u128)(__int128)d[i] * Qa;
            Qa *= qs[i];
        }
        result = from_i128((__int128)acc);
    }

    free(hq); free(Z0); free(W); free(A); free(eta); free(pre);
    free(qs); free(rs); free(d); free(cur); free(Qm);
    return result;
}
