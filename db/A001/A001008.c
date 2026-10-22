/*
 * A001008  a(n) = numerator of harmonic number H(n) = Sum_{i=1..n} 1/i.
 * https://oeis.org/A001008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, nice, easy
 * Formula:   a(n) = numerator of H(n) = S/gcd(S, L), where L = lcm(1..n) and
 *            S = Sum_{i=1..n} L/i; gcd(S, L) is odd (the denominator of H(n)
 *            keeps the full power of 2), and it is found prime by prime from
 *            S mod p^e computed in native modular arithmetic
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 24, u64 n <= 46, u128 n <= 88,
 *            big any n
 * Helpers:   a001008_fixed, a001008_inv
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a001008_fixed.h"
#include "../utils/a001008_inv.h"

seq_t a(idx_t n) {
    long long N, i, j, np = 0;
    char *comp;
    uint64_t *pr, *pe;
    int *ex;
    int fixed;
    seq_t S = 0, g = 1, res;

    if (n <= 1) return (seq_t)1;
    N = (long long)n;
    fixed = a001008_fixed();

    comp = (char *)calloc((size_t)N + 1, 1);
    pr = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)N + 1));
    pe = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)N + 1));
    ex = (int *)malloc(sizeof(int) * ((size_t)N + 1));
    for (i = 2; i <= N; i++) {
        if (!comp[i]) {
            uint64_t q = (uint64_t)i;
            int e = 1;
            for (j = i * i; j <= N; j += i) comp[j] = 1;
            while (q * (uint64_t)i <= (uint64_t)N) { q *= (uint64_t)i; e++; }
            pr[np] = (uint64_t)i;
            pe[np] = q;
            ex[np] = e;
            np++;
        }
    }

    /* S = Sum L/i */
    if (!fixed) {
        seq_t L = 1;
        for (j = 0; j < np; j++) L *= (seq_t)pe[j];
        for (i = 1; i <= N; i++) S += L / (seq_t)i;
    } else {
        /* wrapping arithmetic modulo 2^w */
        for (i = 1; i <= N; i++) {
            seq_t t = 1;
            for (j = 0; j < np; j++) {
                uint64_t p = pr[j], pv = 1, x = (uint64_t)i;
                while (x % p == 0) { x /= p; pv *= p; }
                t *= (seq_t)(pe[j] / pv);
            }
            S += t;
        }
    }

    /* g = gcd(S, L), odd primes only */
    for (j = 1; j < np; j++) {
        uint64_t p = pr[j], m = pe[j], Lp = 1, r = 0;
        long long k;
        int v = 0;
        for (k = 0; k < np; k++)
            if (k != j) Lp = (Lp * (pe[k] % m)) % m;
        for (i = (long long)p; i <= N; i += (long long)p) {
            uint64_t x = (uint64_t)i, pv = 1, term;
            while (x % p == 0) { x /= p; pv *= p; }
            term = ((m / pv) % m) * Lp % m;
            term = term * a001008_inv(x % m, m) % m;
            r = (r + term) % m;
        }
        if (r == 0) v = ex[j];
        else while (r % p == 0) { r /= p; v++; }
        while (v-- > 0) g *= (seq_t)p;
    }

    free(comp); free(pr); free(pe); free(ex);

    if (!fixed) {
        res = S / g;
    } else {
        seq_t inv = g;
        int k;
        for (k = 0; k < 8; k++) inv = inv * (2 - g * inv);
        res = S * inv;
    }
    return res;
}
