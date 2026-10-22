/*
 * A007726  Number of spanning trees of quarter Aztec diamonds of order n.
 * https://oeis.org/A007726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{0<j<k<n} (x_j + x_k), where x_j = 2 -
 *            2*cos(j*Pi/n) are the roots of P(x) = (-1)^(n-1) W_{n-1}(x), W_0
 *            = 1, W_1 = 2-x, W_{k+1} = (2-x)*W_k - W_{k-1}.
 *            Product_{j<k}(x_j+x_k) is the Schur function s_(m-1,...,1,0) of
 *            the roots (m = n-1), so a(n) = det(h_{m-2i+j})_{i,j=1..m}
 *            (Jacobi-T...
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/seq_is_fixed_width.h"

static uint64_t mulm(uint64_t a, uint64_t b, uint64_t p) {
    return (uint64_t)((unsigned __int128)a * b % p);
}

static uint64_t powm(uint64_t b, uint64_t e, uint64_t p) {
    uint64_t r = 1 % p;
    b %= p;
    while (e) {
        if (e & 1) r = mulm(r, b, p);
        b = mulm(b, b, p);
        e >>= 1;
    }
    return r;
}

static int mr_prime(uint64_t n) {
    static const uint64_t bases[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    uint64_t d;
    int s = 0, i, r;
    if (n < 2) return 0;
    for (i = 0; i < 12; i++) {
        if (n % bases[i] == 0) return n == bases[i];
    }
    d = n - 1;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (i = 0; i < 12; i++) {
        uint64_t x = powm(bases[i], d, n);
        if (x == 1 || x == n - 1) continue;
        for (r = 1; r < s; r++) {
            x = mulm(x, x, n);
            if (x == n - 1) break;
        }
        if (r == s) return 0;
    }
    return 1;
}

static uint64_t *g_primes = NULL;
static long g_nprimes = 0;

static void ensure_primes(long k) {
    uint64_t c;
    if (g_nprimes >= k) return;
    g_primes = (uint64_t *)realloc(g_primes, (size_t)k * sizeof(uint64_t));
    c = g_nprimes ? g_primes[g_nprimes - 1] - 2 : (1ULL << 62) - 1;
    while (g_nprimes < k) {
        while (!mr_prime(c)) c -= 2;
        g_primes[g_nprimes++] = c;
        c -= 2;
    }
}

static uint64_t det_mod(uint64_t *A, long m, uint64_t p) {
    uint64_t det = 1;
    long i, j, k, piv;
    for (k = 0; k < m; k++) {
        uint64_t inv;
        piv = -1;
        for (i = k; i < m; i++) {
            if (A[i * m + k] != 0) { piv = i; break; }
        }
        if (piv < 0) return 0;
        if (piv != k) {
            for (j = 0; j < m; j++) {
                uint64_t t = A[k * m + j];
                A[k * m + j] = A[piv * m + j];
                A[piv * m + j] = t;
            }
            det = (p - det) % p;
        }
        det = mulm(det, A[k * m + k], p);
        inv = powm(A[k * m + k], p - 2, p);
        for (i = k + 1; i < m; i++) {
            uint64_t f = A[i * m + k];
            if (f == 0) continue;
            f = mulm(f, inv, p);
            for (j = k; j < m; j++) {
                uint64_t t = mulm(f, A[k * m + j], p);
                uint64_t v = A[i * m + j];
                A[i * m + j] = v >= t ? v - t : v + p - t;
            }
        }
    }
    return det;
}

static uint64_t a_mod(long m, uint64_t p) {
    uint64_t *prev = (uint64_t *)calloc((size_t)(m + 2), sizeof(uint64_t));
    uint64_t *cur = (uint64_t *)calloc((size_t)(m + 2), sizeof(uint64_t));
    uint64_t *nxt = (uint64_t *)calloc((size_t)(m + 2), sizeof(uint64_t));
    uint64_t *h = (uint64_t *)calloc((size_t)(2 * m + 1), sizeof(uint64_t));
    uint64_t *A = (uint64_t *)malloc((size_t)m * (size_t)m * sizeof(uint64_t));
    uint64_t sgn, res;
    long k, i, j;

    /* W_0 = 1, W_1 = 2 - x */
    prev[0] = 1;
    cur[0] = 2 % p;
    cur[1] = p - 1;
    for (k = 1; k < m; k++) {
        for (i = 0; i <= k + 1; i++) {
            uint64_t v = mulm(2, cur[i], p);
            if (i > 0) v = (v + p - cur[i - 1]) % p;
            v = (v + p - prev[i]) % p;
            nxt[i] = v;
        }
        {
            uint64_t *t = prev; prev = cur; cur = nxt; nxt = t;
        }
    }
    /* make monic: P = (-1)^m W_m */
    sgn = (m & 1) ? p - 1 : 1;
    for (i = 0; i <= m; i++) cur[i] = mulm(cur[i], sgn, p);

    h[0] = 1;
    for (k = 1; k <= 2 * m; k++) {
        uint64_t s = 0;
        long lim = k < m ? k : m;
        for (i = 1; i <= lim; i++) {
            s += mulm(cur[m - i], h[k - i], p);
            if (s >= p) s -= p;
        }
        h[k] = s ? p - s : 0;
    }

    for (i = 0; i < m; i++) {
        for (j = 0; j < m; j++) {
            long idx = m - 2 * i + j - 1;
            A[i * m + j] = idx < 0 ? 0 : h[idx];
        }
    }
    res = det_mod(A, m, p);

    free(prev); free(cur); free(nxt); free(h); free(A);
    return res;
}

seq_t a(idx_t n) {
    long m, K, i, j;
    double B = 0.0, pi = acos(-1.0);
    uint64_t *r, *d;
    seq_t res;

    if (n <= 2) return 1;
    m = (long)(n - 1);

    /* upper estimate of log2(a(n)) to decide how many primes are needed */
    for (i = 1; i <= m; i++) {
        double ci = cos((double)i * pi / (double)n);
        for (j = i + 1; j <= m; j++) {
            B += log2(4.0 - 2.0 * ci - 2.0 * cos((double)j * pi / (double)n));
        }
    }
    K = (long)((B * 1.01 + 64.0) / 61.0) + 1;
    if (seq_is_fixed_width() && K > 4) K = 4;

    ensure_primes(K);
    r = (uint64_t *)malloc((size_t)K * sizeof(uint64_t));
    d = (uint64_t *)malloc((size_t)K * sizeof(uint64_t));
    for (i = 0; i < K; i++) r[i] = a_mod(m, g_primes[i]);

    /* Garner mixed-radix digits */
    for (i = 0; i < K; i++) {
        uint64_t p = g_primes[i];
        uint64_t v = r[i] % p;
        for (j = 0; j < i; j++) {
            uint64_t dj = d[j] % p;
            uint64_t inv = powm(g_primes[j] % p, p - 2, p);
            v = v >= dj ? v - dj : v + p - dj;
            v = mulm(v, inv, p);
        }
        d[i] = v;
    }

    res = (seq_t)d[K - 1];
    for (i = K - 2; i >= 0; i--) {
        res = res * (seq_t)g_primes[i] + (seq_t)d[i];
    }

    free(r);
    free(d);
    return res;
}
