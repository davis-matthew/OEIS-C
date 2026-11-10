/*
 * A069785  a(n) = A061680(n!).
 * https://oeis.org/A069785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A061680(n!) = gcd(d((n!)^2), d(n!)), d = number of
 *            divisors; exponents of n! by Legendre's formula, gcd taken on
 *            prime-exponent counts
 * Range:     u8 n <= 17, u16 n <= 51, u32 n <= 90, u64 n <= 241,
 *            u128 n <= 548, big any n
 * Validated: standard, pass, 69 terms through n = 843 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static void A069785_add(unsigned *cnt, uint64_t v) {
    uint64_t q;
    for (q = 2; q * q <= v; q++) {
        while (v % q == 0) {
            cnt[q]++;
            v /= q;
        }
    }
    if (v > 1) cnt[v]++;
}

seq_t a(idx_t n) {
    size_t N = (size_t)n;
    size_t L = 2 * N + 3;
    unsigned *c1 = (unsigned *)calloc(L, sizeof(unsigned));
    unsigned *c2 = (unsigned *)calloc(L, sizeof(unsigned));
    char *comp = (char *)calloc(N + 2, 1);
    size_t p, q;
    seq_t res = 1;

    for (p = 2; p <= N; p++) {
        if (comp[p]) continue;
        for (q = p * p; q <= N; q += p) comp[q] = 1;
        uint64_t e = 0, t = (uint64_t)N / p;
        while (t > 0) {
            e += t;
            t /= p;
        }
        A069785_add(c1, e + 1);
        A069785_add(c2, 2 * e + 1);
    }
    for (q = 2; q < L; q++) {
        unsigned m = c1[q] < c2[q] ? c1[q] : c2[q];
        while (m--) res *= (seq_t)q;
    }
    free(c1);
    free(c2);
    free(comp);
    return res;
}
