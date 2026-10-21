/*
 * A088432  Number of ways to write n as n = u*v*w with 1 <= u < v <= w.
 * https://oeis.org/A088432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = count of triples (u,v,w) such that 1 <= u < v <= w and
 *            u*v*w = n
 * Range:     u8 n <= 3003, u16 n <= 3003, u32 n <= 3003, u64 n <= 3003,
 *            u128 n <= 3003, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static void factor_u64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) { n /= p; ++e; }
            pr[c]   = p;
            exp[c]  = e;
            ++c;
        }
    }
    if (n > 1 && c < 32) { pr[c] = n; exp[c] = 1; ++c; }
    *cnt = c;
}

static void gen_divs_rec(const uint64_t *pr, const uint64_t *exp, size_t cnt,
                         size_t idx, uint64_t cur, uint64_t *divs, size_t *divc)
{
    if (idx == cnt) {
        divs[*divc] = cur;
        ++(*divc);
        return;
    }
    uint64_t p = pr[idx];
    uint64_t val = 1;
    for (uint64_t e = 0; e <= exp[idx]; ++e) {
        gen_divs_rec(pr, exp, cnt, idx + 1, cur * val, divs, divc);
        val *= p;
    }
}

static void gen_divs(const uint64_t *pr, const uint64_t *exp, size_t cnt,
                     uint64_t *divs, size_t *divc)
{
    *divc = 0;
    gen_divs_rec(pr, exp, cnt, 0, 1, divs, divc);
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;
    uint64_t nu = (uint64_t)n;
    uint64_t primes[32];
    uint64_t exps[32];
    size_t pcnt;
    factor_u64(nu, primes, exps, &pcnt);

    uint64_t divs[256];
    size_t dcnt;
    gen_divs(primes, exps, pcnt, divs, &dcnt);

    seq_t count = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t u = divs[i];
        if (u == 0) continue;
        for (size_t j = 0; j < dcnt; ++j) {
            uint64_t v = divs[j];
            if (u >= v) continue;            // require u < v
            if (u > nu / v) continue;        // avoid overflow / check divisibility
            if ((nu / u) % v != 0) continue; // ensure integer w
            uint64_t w = nu / (u * v);
            if (v <= w) {
                ++count;
            }
        }
    }
    return count;
}
