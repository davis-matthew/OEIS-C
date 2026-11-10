/*
 * A229300  Numbers n such that A031971(1806*n) == n (mod 1806*n).
 * https://oeis.org/A229300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k>=1 with (Sum_{j=1..m} j^m) == k (mod m), m =
 *            1806*k; found by search, using multiplicativity of j^m mod m
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A229300_pm(uint64_t b, uint64_t e, uint64_t m)
{
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = r * b % m;
        b = b * b % m;
        e >>= 1;
    }
    return r;
}

static int A229300_ok(uint64_t k)
{
    uint64_t m = 1806ULL * k;
    uint64_t *pw = (uint64_t *)malloc((size_t)(m + 1) * sizeof(uint64_t));
    uint32_t *lp = (uint32_t *)calloc((size_t)(m + 1), sizeof(uint32_t));
    uint32_t *pr = (uint32_t *)malloc((size_t)(m / 2 + 16) * sizeof(uint32_t));
    size_t np = 0;
    uint64_t sum = 1 % m;
    pw[0] = 0;
    pw[1] = 1 % m;
    for (uint64_t i = 2; i <= m; ++i) {
        if (lp[i] == 0) {
            lp[i] = (uint32_t)i;
            pr[np++] = (uint32_t)i;
            pw[i] = A229300_pm(i, m, m);
        } else {
            pw[i] = pw[lp[i]] * pw[i / lp[i]] % m;
        }
        for (size_t j = 0; j < np; ++j) {
            uint64_t p = pr[j];
            if (p > lp[i] || p * i > m) break;
            lp[p * i] = (uint32_t)p;
        }
        sum += pw[i];
        if (sum >= m) sum -= m;
    }
    free(pw);
    free(lp);
    free(pr);
    return sum == k % m;
}

seq_t a(idx_t n)
{
    static uint64_t *terms = 0;
    static size_t cnt = 0, cap = 0;
    static uint64_t cur = 0;
    if (n == 0) return (seq_t)0;
    while (cnt < (size_t)n) {
        ++cur;
        if (A229300_ok(cur)) {
            if (cnt == cap) {
                cap = cap ? cap * 2 : 64;
                terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
            }
            terms[cnt++] = cur;
        }
    }
    return (seq_t)terms[n - 1];
}
