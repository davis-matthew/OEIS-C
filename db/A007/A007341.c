/*
 * A007341  Number of spanning trees in n X n grid.
 * https://oeis.org/A007341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = Resultant( Q(x), Q(-x) ) up to sign = prod_{j,k=1..n-1}
 *            (L_j + L_k), where L_k = 2 - 2*cos(Pi*k/n) are the nonzero
 *            eigenvalues of the path Laplacian and Q(x) = prod (x - L_k) =
 *            (-1)^(n-1) * U(n-1, 1 - x/2) (Chebyshev U of the second kind).
 *            The resultant is computed modulo many large primes...
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 5, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"

typedef unsigned long long u64;
typedef unsigned __int128 u128;

static u64 mulm(u64 a, u64 b, u64 p) { return (u64)((u128)a * b % p); }

static u64 powm(u64 a, u64 e, u64 p) {
    u64 r = 1 % p;
    a %= p;
    while (e) {
        if (e & 1) r = mulm(r, a, p);
        a = mulm(a, a, p);
        e >>= 1;
    }
    return r;
}

static bool mr_prime(u64 n) {
    static const u64 bases[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    int i, s = 0;
    u64 d = n - 1;
    if (n < 2) return false;
    for (i = 0; i < 12; i++) {
        if (n == bases[i]) return true;
        if (n % bases[i] == 0) return false;
    }
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (i = 0; i < 12; i++) {
        u64 x = powm(bases[i], d, n);
        int r;
        bool comp = true;
        if (x == 1 || x == n - 1) continue;
        for (r = 1; r < s; r++) {
            x = mulm(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

static u64 *primes = NULL;
static size_t nprimes = 0, capprimes = 0;
static u64 next_cand = (1ULL << 62) - 1;

static void ensure_primes(size_t k) {
    while (nprimes < k) {
        while (!mr_prime(next_cand)) next_cand -= 2;
        if (nprimes == capprimes) {
            capprimes = capprimes ? 2 * capprimes : 64;
            primes = (u64 *)realloc(primes, capprimes * sizeof(u64));
        }
        primes[nprimes++] = next_cand;
        next_cand -= 2;
    }
}

/* prod_{j,k=1..d} (L_j + L_k) mod p, with d = n-1 */
static u64 res_mod(u64 p, long long d) {
    u64 *v0 = (u64 *)calloc((size_t)d + 2, sizeof(u64));
    u64 *v1 = (u64 *)calloc((size_t)d + 2, sizeof(u64));
    u64 *v2 = (u64 *)calloc((size_t)d + 2, sizeof(u64));
    u64 *A, *B, *T;
    long long m, i, j, degA, degB;
    u64 res = 1;

    /* V_m(x) = U_m(1 - x/2): V_0 = 1, V_1 = 2 - x, V_{m+1} = (2 - x) V_m - V_{m-1} */
    v0[0] = 1;
    if (d >= 1) {
        v1[0] = 2 % p;
        v1[1] = p - 1;
        for (m = 1; m < d; m++) {
            for (i = 0; i <= m + 1; i++) {
                u64 t = 0;
                if (i <= m) t = mulm(2, v1[i], p);
                if (i >= 1) t = (t + p - v1[i - 1]) % p;
                if (i <= m - 1) t = (t + p - v0[i]) % p;
                v2[i] = t;
            }
            T = v0; v0 = v1; v1 = v2; v2 = T;
        }
        T = v0; v0 = v1; v1 = T;   /* v0 = V_d */
    }

    /* A = Q(x) = (-1)^d V_d(x),  B = V_d(-x) = prod (x + L_k); both monic */
    A = v1; B = v2;
    for (i = 0; i <= d; i++) {
        u64 c = v0[i] % p;
        A[i] = (d & 1) ? (p - c) % p : c;
        B[i] = (i & 1) ? (p - c) % p : c;
    }
    degA = d; degB = d;

    for (;;) {
        u64 inv, lb;
        long long degR;
        if (degB == 0) {
            res = mulm(res, powm(B[0], (u64)degA, p), p);
            break;
        }
        lb = B[degB];
        inv = powm(lb, p - 2, p);
        for (i = degA; i >= degB; i--) {
            u64 c = mulm(A[i], inv, p);
            if (c == 0) continue;
            for (j = 0; j <= degB; j++) {
                long long idx = i - degB + j;
                A[idx] = (A[idx] + p - mulm(c, B[j], p)) % p;
            }
        }
        degR = degB - 1;
        while (degR >= 0 && A[degR] == 0) degR--;
        if (degR < 0) { res = 0; break; }
        if ((degA & 1) && (degB & 1)) res = (p - res) % p;
        res = mulm(res, powm(lb, (u64)(degA - degR), p), p);
        T = A; A = B; B = T;
        degA = degB; degB = degR;
    }

    free(v0); free(v1); free(v2);
    return res;
}

seq_t a(idx_t n) {
    long long d = (long long)n - 1;
    u64 bits = (u64)(1683ULL * n * n / 1000ULL) + 64;
    size_t k, i, j;
    u64 *r, *t;
    seq_t v = 0;

    if (seq_is_fixed_width()) {
        u64 w = 8ULL * sizeof(seq_t);
        double lower = 1.6 * (double)n * (double)n - 4.0 * (double)n - 10.0;
        if (lower > (double)w) return 0;   /* cannot fit in seq_t */
        if (bits > w + 1) bits = w + 1;
    }

    k = (size_t)(bits / 61) + 1;
    ensure_primes(k);

    r = (u64 *)malloc(k * sizeof(u64));
    t = (u64 *)malloc(k * sizeof(u64));
    for (i = 0; i < k; i++) r[i] = res_mod(primes[i], d);

    /* Garner: mixed-radix digits */
    for (i = 0; i < k; i++) {
        u64 p = primes[i], val = 0, c = 1 % p;
        for (j = 0; j < i; j++) {
            val = (val + mulm(t[j] % p, c, p)) % p;
            c = mulm(c, primes[j] % p, p);
        }
        t[i] = mulm((r[i] + p - val) % p, powm(c, p - 2, p), p);
    }

    for (i = k; i-- > 0;) {
        v = v * (idx_t)primes[i];
        v += (idx_t)t[i];
    }

    free(r); free(t);
    return v;
}
