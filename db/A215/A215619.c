/*
 * A215619  a(n) is the number of consecutive terms of A100071, beginning with
 *          index n, which are divisible by n.
 * https://oeis.org/A215619
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of consecutive j>=0 with n | A100071(n+j), where
 *            A100071(m) = m*binomial(m-1, floor((m-1)/2)); checked via p-adic
 *            valuations (Legendre), scanning j = 0..n
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 96 terms through n = 901 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t a215619_legendre(uint64_t m, uint64_t p)
{
    uint64_t s = 0;
    while (m) {
        m /= p;
        s += m;
    }
    return s;
}

static uint64_t a215619_val(uint64_t m, uint64_t p)
{
    uint64_t v = 0;
    while (m % p == 0) {
        m /= p;
        v++;
    }
    return v;
}

seq_t a(idx_t n)
{
    uint64_t pr[32];
    uint64_t ex[32];
    int cnt = 0;
    uint64_t t = (uint64_t)n;
    for (uint64_t p = 2; p * p <= t; p++) {
        if (t % p == 0) {
            uint64_t e = 0;
            while (t % p == 0) {
                t /= p;
                e++;
            }
            pr[cnt] = p;
            ex[cnt] = e;
            cnt++;
        }
    }
    if (t > 1) {
        pr[cnt] = t;
        ex[cnt] = 1;
        cnt++;
    }

    uint64_t count = 0;
    for (uint64_t j = 0; j <= (uint64_t)n; j++) {
        uint64_t m = (uint64_t)n + j;
        uint64_t top = m - 1;
        uint64_t k = top / 2;
        uint64_t r = top - k;
        int ok = 1;
        for (int i = 0; i < cnt; i++) {
            uint64_t p = pr[i];
            uint64_t v = a215619_val(m, p);
            v += a215619_legendre(top, p) - a215619_legendre(k, p) - a215619_legendre(r, p);
            if (v < ex[i]) {
                ok = 0;
                break;
            }
        }
        if (!ok) break;
        count++;
    }
    return (seq_t)count;
}
