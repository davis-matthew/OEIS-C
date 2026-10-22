/*
 * A006233  Denominators of Cauchy numbers of first type.
 * https://oeis.org/A006233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, nice, easy
 * Formula:   a(n) = denominator of Sum_{k=0..n} Stirling1(n,k)/(k+1)
 *            (Stirling1 signed). The denominator divides lcm(1..n+1), so for
 *            each prime p <= n+1 we find v_p of the sum. If p^2 <= n+1, we
 *            compute the falling factorial mod p^e. If p^2 > n+1, we use
 *            (x)_n == (x^p-x)^q * (x)_r (mod p), where n = q*p + r
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 71, u64 n <= 311,
 *            u128 n <= 1000, big any n
 * Helpers:   inv_mod_ll
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/inv_mod_ll.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    uint64_t N = (uint64_t)n + 1;
    char *comp = (char *)calloc(N + 1, 1);
    uint64_t i, j;
    for (i = 2; i * i <= N; i++)
        if (!comp[i])
            for (j = i * i; j <= N; j += i) comp[j] = 1;

    seq_t res = 1;

    /* small primes: p^2 <= N */
    uint64_t ns = 0;
    for (i = 2; i * i <= N; i++) if (!comp[i]) ns++;
    uint64_t *ps = (uint64_t *)malloc((ns + 1) * sizeof(uint64_t));
    uint64_t *pes = (uint64_t *)malloc((ns + 1) * sizeof(uint64_t));
    int *es = (int *)malloc((ns + 1) * sizeof(int));
    ns = 0;
    for (i = 2; i * i <= N; i++) {
        if (comp[i]) continue;
        uint64_t pe = i; int e = 1;
        while (pe * i <= N) { pe *= i; e++; }
        ps[ns] = i; pes[ns] = pe; es[ns] = e; ns++;
    }

    uint64_t *c = (uint64_t *)malloc((N + 1) * sizeof(uint64_t));
    uint64_t s = 0;
    while (s < ns) {
        uint64_t g = 1, t = s;
        while (t < ns && g <= ((uint64_t)1 << 62) / pes[t]) { g *= pes[t]; t++; }
        uint64_t m = g;
        /* coefficients of x(x-1)...(x-n+1) mod m */
        for (i = 0; i <= N; i++) c[i] = 0;
        c[0] = 1 % m;
        uint64_t k;
        for (k = 0; k < (uint64_t)n; k++) {
            uint64_t w = (m - k % m) % m;
            uint64_t wp = (uint64_t)((((unsigned __int128)w) << 64) / m);
            uint64_t idx;
            for (idx = k + 1; idx >= 1; idx--) {
                uint64_t tv = c[idx];
                uint64_t qq = (uint64_t)(((unsigned __int128)tv * wp) >> 64);
                uint64_t r = tv * w - qq * m;
                if (r >= m) r -= m;
                r += c[idx - 1];
                if (r >= m) r -= m;
                c[idx] = r;
            }
            {
                uint64_t tv = c[0];
                uint64_t qq = (uint64_t)(((unsigned __int128)tv * wp) >> 64);
                uint64_t r = tv * w - qq * m;
                if (r >= m) r -= m;
                c[0] = r;
            }
        }
        uint64_t u2;
        for (u2 = s; u2 < t; u2++) {
            uint64_t p = ps[u2], pe = pes[u2];
            int e = es[u2];
            uint64_t S = 0;
            uint64_t kp1;
            for (kp1 = p; kp1 <= N; kp1 += p) {
                uint64_t u = kp1; int v = 0;
                while (u % p == 0) { u /= p; v++; }
                uint64_t f = 1; int z;
                for (z = 0; z < e - v; z++) f *= p;
                uint64_t term = (c[kp1 - 1] % pe) * f % pe;
                term = term * (uint64_t)inv_mod_ll((long long)(u % pe), (long long)pe) % pe;
                S = (S + term) % pe;
            }
            if (S != 0) {
                int v = 0;
                while (S % p == 0) { S /= p; v++; }
                int z;
                for (z = 0; z < e - v; z++) res *= (seq_t)p;
            }
        }
        s = t;
    }

    /* large primes: p^2 > N, exponent at most 1 */
    uint64_t *E = (uint64_t *)malloc((N + 2) * sizeof(uint64_t));
    uint64_t p;
    for (p = 2; p <= N; p++) {
        if (comp[p] || p * p <= N) continue;
        uint64_t q = (uint64_t)n / p, r = (uint64_t)n % p;
        long long D = (long long)r - (long long)p + (long long)q + 1;
        if (D < 0) continue;
        long long d;
        E[0] = 1;
        for (d = 1; d <= D; d++) E[d] = 0;
        uint64_t cc;
        for (cc = 1; cc < r; cc++)
            for (d = D; d >= 1; d--)
                E[d] = (E[d] + cc * E[d - 1]) % p;
        uint64_t S = 0, b = 1;
        uint64_t jmax = q < (uint64_t)D ? q : (uint64_t)D;
        for (j = 0; j <= jmax; j++) {
            uint64_t dd = (uint64_t)D - j;
            uint64_t iv = (uint64_t)inv_mod_ll((long long)(j + 1), (long long)p);
            uint64_t term = b * E[dd] % p * iv % p;
            if (((q - j) + dd) & 1) term = (p - term) % p;
            S = (S + term) % p;
            b = b * ((q - j) % p) % p * iv % p;
        }
        if (S != 0) res *= (seq_t)p;
    }

    free(E); free(c); free(ps); free(pes); free(es); free(comp);
    return res;
}
