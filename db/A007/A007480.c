/*
 * A007480  a(n) = denominator of sum_{k=1..n} k^(-4).
 * https://oeis.org/A007480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = denominator of Sum_{k=1..n} 1/k^4 = L / gcd(N, L), where
 *            L = lcm(1..n)^4 and N = Sum_{k=1..n} L/k^4; for each prime p <=
 *            n the exponent p^v dividing N is found from N mod p^(v_p(L))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   a007480_inv, a007480_mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a007480_inv.h"
#include "../utils/a007480_mulmod.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    if (n < 2) return result;
    char *comp = (char *)calloc((size_t)n + 1, 1);
    for (uint64_t p = 2; p <= n; p++) {
        if (comp[p]) continue;
        for (uint64_t q = p * p; q <= n; q += p) comp[q] = 1;

        /* a = max j with p^j <= n, e = 4a, M = p^e */
        uint64_t aexp = 0, pj = 1;
        while (pj <= n / p) { pj *= p; aexp++; }
        uint64_t e = 4 * aexp;
        uint64_t M = 1;
        for (uint64_t i = 0; i < e; i++) M *= p;

        /* p4[i] = p^(4i) */
        uint64_t p4[64];
        p4[0] = 1;
        for (uint64_t i = 1; i <= aexp; i++) p4[i] = p4[i - 1] * p * p * p * p;

        uint64_t S = 0;
        for (uint64_t k = p; k <= n; k += p) {
            uint64_t m = k, j = 0;
            while (m % p == 0) { m /= p; j++; }
            uint64_t iv = a007480_inv(m, M);
            uint64_t iv2 = a007480_mulmod(iv, iv, M);
            uint64_t iv4 = a007480_mulmod(iv2, iv2, M);
            uint64_t term = a007480_mulmod(p4[aexp - j] % M, iv4, M);
            S += term;
            if (S >= M) S -= M;
        }

        uint64_t v = 0;
        if (S == 0) v = e;
        else while (S % p == 0 && v < e) { S /= p; v++; }

        for (uint64_t i = v; i < e; i++) result *= (seq_t)p;
    }
    free(comp);
    return result;
}
