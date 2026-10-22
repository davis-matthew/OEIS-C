/*
 * A004206  Number of permutations of length n with spread 2.
 * https://oeis.org/A004206
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ permutations s of Z_n : Sum_{i} i*s(i) == 2 (mod n) }
 *            = (1/n) * Sum_{d|n} c_{n/d}(2) * per(M_d), where M_d[i][j] =
 *            w^(d*i*j), w is a primitive n-th root of unity, and c_q(2) is
 *            the Ramanujan sum. Each permanent is computed mod several primes
 *            P == 1 (mod n) by a DP over column-class coun...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   is_prime, perm_mod, pm
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/perm_mod.h"
#include "../utils/pm.h"

static int mobius(uint64_t x) {
    int s = 1;
    for (uint64_t q = 2; q * q <= x; q++) {
        if (x % q == 0) {
            x /= q;
            if (x % q == 0) return 0;
            s = -s;
        }
    }
    if (x > 1) s = -s;
    return s;
}

/* Ramanujan sum c_m(2) */
static int ram2(uint64_t m) {
    if (m % 2) return mobius(m);
    if ((m / 2) % 2) return mobius(m / 2);
    return 2 * mobius(m / 2);
}

seq_t a(idx_t n) {
    if (n < 3) return 0;
    uint64_t B = 0;
    for (uint64_t i = 2; i <= n; i++) {
        uint64_t t = i, bl = 0;
        while (t) { bl++; t >>= 1; }
        B += bl;
    }
    idx_t r = B / 30 + 1;
    uint64_t *P = (uint64_t *)malloc(r * sizeof(uint64_t));
    uint64_t *x = (uint64_t *)malloc(r * sizeof(uint64_t));
    uint64_t *v = (uint64_t *)malloc(r * sizeof(uint64_t));
    uint64_t *pw = (uint64_t *)malloc(n * sizeof(uint64_t));

    /* prime factors of n */
    uint64_t pf[64]; int npf = 0;
    {
        uint64_t t = n;
        for (uint64_t q = 2; q * q <= t; q++)
            if (t % q == 0) { pf[npf++] = q; while (t % q == 0) t /= q; }
        if (t > 1) pf[npf++] = t;
    }

    idx_t found = 0;
    uint64_t k = ((1ULL << 31) - 1) / n;
    while (found < r) {
        uint64_t p = k * n + 1;
        k--;
        if (p < (1ULL << 30) && found == 0 && k == 0) break;
        if (!is_prime(p)) continue;
        P[found++] = p;
    }

    for (idx_t i = 0; i < r; i++) {
        uint64_t p = P[i], om = 0;
        for (uint64_t g = 2;; g++) {
            uint64_t w = pm(g, (p - 1) / n, p);
            int ok = 1;
            for (int j = 0; j < npf; j++)
                if (pm(w, n / pf[j], p) == 1) { ok = 0; break; }
            if (ok) { om = w; break; }
        }
        pw[0] = 1;
        for (idx_t e = 1; e < n; e++) pw[e] = pw[e - 1] * om % p;
        uint64_t T = 0;
        for (idx_t d = 1; d <= n; d++) {
            if (n % d) continue;
            int c = ram2(n / d);
            if (c == 0) continue;
            uint64_t per = perm_mod(n, d, pw, p);
            uint64_t term = (uint64_t)(c < 0 ? -c : c) * per % p;
            if (c < 0) term = (p - term) % p;
            T = (T + term) % p;
        }
        x[i] = T * pm(n % p, p - 2, p) % p;
    }

    /* Garner */
    for (idx_t i = 0; i < r; i++) {
        uint64_t t = x[i];
        for (idx_t j = 0; j < i; j++) {
            t = (t + P[i] - v[j] % P[i]) % P[i];
            t = t * pm(P[j] % P[i], P[i] - 2, P[i]) % P[i];
        }
        v[i] = t;
    }
    seq_t res = (seq_t)v[r - 1];
    for (idx_t i = r - 1; i-- > 0;) {
        res = res * (seq_t)P[i] + (seq_t)v[i];
    }

    free(P); free(x); free(v); free(pw);
    return res;
}
