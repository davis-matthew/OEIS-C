/*
 * A004003  Number of domino tilings (or dimer coverings) of a 2n X 2n square.
 * https://oeis.org/A004003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2^n * B(n)^2 with B(n) = Product_{1<=j<k<=n} (y_j + y_k),
 *            y_j = 4*cos(j*Pi/(2n+1))^2 = 2 + w^j + w^(-j), w a primitive
 *            (2n+1)-th root of unity (from a(n) = Product_{j,k} (y_j + y_k)
 *            and Product_j y_j = 1). B(n) is computed modulo many primes p ==
 *            1 (mod 2n+1) and recovered by CRT (Garner)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   seq_is_fixed_width, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/u128_to_seq.h"
#include "../utils/seq_is_fixed_width.h"

typedef uint64_t a4003_u64;
typedef unsigned __int128 a4003_u128;

static a4003_u64 a4003_mm(a4003_u64 a, a4003_u64 b, a4003_u64 p)
{
    return (a4003_u64)((a4003_u128)a * b % p);
}

static a4003_u64 a4003_pw(a4003_u64 b, a4003_u64 e, a4003_u64 p)
{
    a4003_u64 r = 1;
    b %= p;
    while (e) {
        if (e & 1) r = a4003_mm(r, b, p);
        b = a4003_mm(b, b, p);
        e >>= 1;
    }
    return r;
}

static bool a4003_mr(a4003_u64 n)
{
    static const a4003_u64 bases[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    a4003_u64 d = n - 1;
    int s = 0, i, r;
    if (n < 2) return false;
    for (i = 0; i < 12; i++) {
        if (n == bases[i]) return true;
        if (n % bases[i] == 0) return false;
    }
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (i = 0; i < 12; i++) {
        a4003_u64 x = a4003_pw(bases[i], d, n);
        if (x == 1 || x == n - 1) continue;
        for (r = 1; r < s; r++) {
            x = a4003_mm(x, x, n);
            if (x == n - 1) break;
        }
        if (r == s) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    a4003_u64 m, mm_, qs[16], nb, k, *P, *R, *V, *y;
    int nq = 0;
    size_t kc, cnt, i, j, jj;

    if (n == 0) return (seq_t)1;
    if (seq_is_fixed_width() && n > 40) return (seq_t)0; /* cannot fit anyway */

    m = 2 * (a4003_u64)n + 1;
    mm_ = m;
    for (a4003_u64 q = 2; q * q <= mm_; q++) {
        if (mm_ % q == 0) {
            qs[nq++] = q;
            while (mm_ % q == 0) mm_ /= q;
        }
    }
    if (mm_ > 1) qs[nq++] = mm_;

    nb = (m * m * 211) / 1000 + 64;   /* upper bound on log2 B(n) */
    kc = (size_t)(nb / 61 + 1);

    P = (a4003_u64 *)malloc(kc * sizeof(a4003_u64));
    R = (a4003_u64 *)malloc(kc * sizeof(a4003_u64));
    V = (a4003_u64 *)malloc(kc * sizeof(a4003_u64));
    y = (a4003_u64 *)malloc((size_t)n * sizeof(a4003_u64));

    k = (((a4003_u64)1) << 62) / m;
    cnt = 0;
    while (cnt < kc) {
        a4003_u64 p = k * m + 1;
        k--;
        if (a4003_mr(p)) P[cnt++] = p;
    }

    for (i = 0; i < kc; i++) {
        a4003_u64 p = P[i], w = 0, g, res = 1, wj;
        for (g = 2;; g++) {
            int t;
            bool ok = true;
            w = a4003_pw(g, (p - 1) / m, p);
            if (w == 1) continue;
            for (t = 0; t < nq; t++)
                if (a4003_pw(w, m / qs[t], p) == 1) { ok = false; break; }
            if (ok) break;
        }
        wj = 1;
        for (j = 0; j < (size_t)n; j++) {
            wj = a4003_mm(wj, w, p);                       /* w^(j+1) */
            a4003_u64 winv = a4003_pw(wj, m - 1, p);       /* w^-(j+1) */
            a4003_u64 v = (2 + wj) % p;
            v = (v + winv) % p;
            y[j] = v;
        }
        for (j = 0; j < (size_t)n; j++)
            for (jj = j + 1; jj < (size_t)n; jj++) {
                a4003_u64 s = y[j] + y[jj];
                if (s >= p) s -= p;
                res = a4003_mm(res, s, p);
            }
        R[i] = res;
    }

    /* Garner: B = V[0] + V[1]*P[0] + V[2]*P[0]*P[1] + ... */
    for (i = 0; i < kc; i++) {
        a4003_u64 p = P[i], s = 0, pr = 1, t;
        for (j = 0; j < i; j++) {
            s = (s + a4003_mm(V[j] % p, pr, p)) % p;
            pr = a4003_mm(pr, P[j] % p, p);
        }
        t = (R[i] + p - s) % p;
        V[i] = a4003_mm(t, a4003_pw(pr, p - 2, p), p);
    }

    if (seq_is_fixed_width()) {
        a4003_u128 x = V[kc - 1];
        for (i = kc - 1; i-- > 0;)
            x = x * P[i] + V[i];
        x = x * x;
        if (n >= 128) x = 0; else x <<= n;
        free(P); free(R); free(V); free(y);
        return u128_to_seq(x);
    } else {
        seq_t x = u128_to_seq((a4003_u128)V[kc - 1]);
        for (i = kc - 1; i-- > 0;)
            x = x * u128_to_seq((a4003_u128)P[i]) + u128_to_seq((a4003_u128)V[i]);
        x = x * x;
        x <<= n;
        free(P); free(R); free(V); free(y);
        return x;
    }
}
