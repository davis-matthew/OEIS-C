/*
 * A006634  a(n) = 3*binomial(4*n+8, n)/(n+3).
 * https://oeis.org/A006634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*binomial(4*n+8, n)/(n+3), evaluated as
 *            3*(4n+8)!/(n!*(3n+8)!*(n+3)) via prime factorization (Legendre's
 *            formula)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 198 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t legendre_exp(uint64_t m, uint64_t p) {
    uint64_t s = 0;
    while (m) {
        m /= p;
        s += m;
    }
    return s;
}

seq_t a(idx_t n) {
    uint64_t m = 4 * (uint64_t)n + 8;
    uint64_t k1 = (uint64_t)n;
    uint64_t k2 = 3 * (uint64_t)n + 8;
    uint64_t d = (uint64_t)n + 3;
    char *comp = (char *)calloc((size_t)(m + 1), 1);
    seq_t result = 1;
    uint64_t p, q;

    for (p = 2; p <= m; p++) {
        if (comp[p]) continue;
        for (q = p * p; q <= m; q += p) comp[q] = 1;

        int64_t e = (int64_t)legendre_exp(m, p)
                  - (int64_t)legendre_exp(k1, p)
                  - (int64_t)legendre_exp(k2, p);
        uint64_t t = d;
        while (t % p == 0) {
            t /= p;
            e--;
        }
        if (p == 3) e++;

        while (e > 0) {
            result *= (seq_t)p;
            e--;
        }
    }
    free(comp);
    return result;
}
