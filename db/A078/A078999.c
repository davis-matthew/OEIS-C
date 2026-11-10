/*
 * A078999  Coefficients A_n for the s=4 tennis ball problem.
 * https://oeis.org/A078999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( A078995(n+1) - (5*n+6) * A002293(n+1) ) / 2, where
 *            A078995(m) = Sum_{k=0..m} C(4k,k)*C(4(m-k),m-k), A002293(m) =
 *            C(4m,m)/(3m+1). The symmetric sum is folded so the halving is
 *            done exactly without overflow
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 19 (2026-11-07)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/mem.h"

static uint64_t A078999_leg(uint64_t m, uint64_t p) {
    uint64_t e = 0;
    while (m) { m /= p; e += m; }
    return e;
}

/* A002293(n) = C(4n,n)/(3n+1) via prime factorization */
static seq_t A078999_A002293(idx_t n) {
    seq_t r = 1;
    uint64_t N, i, j, p;
    unsigned char *comp;
    if (n == 0) return r;
    N = 4 * (uint64_t)n;
    comp = (unsigned char *)calloc((size_t)N + 1, 1);
    for (i = 2; i <= N; i++) {
        if (comp[i]) continue;
        for (j = i * i; j <= N; j += i) comp[j] = 1;
        p = i;
        {
            uint64_t e = A078999_leg(N, p) - A078999_leg((uint64_t)n, p)
                       - A078999_leg(3 * (uint64_t)n + 1, p);
            while (e--) r *= (seq_t)p;
        }
    }
    free(comp);
    return r;
}

seq_t a(idx_t n)
{
    size_t m = (size_t)n + 1;
    seq_t *b = mem_new(seq_t, m + 1);
    seq_t res = 0;
    seq_t cat, P2, Q2, c;
    size_t k;
    idx_t f;

    for (k = 0; k <= m; ++k)
        b[k] = binomial((idx_t)(4 * k), (idx_t)k);

    /* half of the symmetric sum: pairs k < m-k */
    for (k = 0; 2 * k < m; ++k)
        res += b[k] * b[m - k];

    /* P2 = floor(b[m/2]^2 / 2) if m even, else 0 */
    P2 = 0;
    if (m % 2 == 0) {
        seq_t x = b[m / 2];
        c = x / 2;
        P2 = x * c;
        if (x % 2 != 0) P2 += c;
    }

    /* Q2 = floor((5n+6)*cat / 2) */
    cat = A078999_A002293((idx_t)m);
    f = 5 * n + 6;
    if (f % 2 == 0) {
        Q2 = (seq_t)(f / 2) * cat;
    } else {
        Q2 = (seq_t)((f - 1) / 2) * cat + cat / 2;
    }

    res += P2;
    res -= Q2;
    mem_free(b);
    return res;
}
