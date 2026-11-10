/*
 * A231900  Omit the 1s from first differences of A084663.
 * https://oeis.org/A231900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th term > 1 of b(m) = gcd(A084663(m-1), m), where
 *            A084663(1) = 8, A084663(m) = A084663(m-1) + gcd(A084663(m-1),
 *            m). Shortcut: while differences are 1, A084663(m-1) - (m-1) = x
 *            stays fixed, so gcd(A084663(m-1), m) = gcd(D, m) with D = x - 1.
 *            The next jump is at the smallest m > k divisibl...
 * Range:     u8 n <= 13, u16 n <= 66, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Validated: standard, pass, 86 terms through n = 118 (2026-11-07)
 * Helpers:   gcd, is_prime_u64, mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/mulmod.h"
#include "../utils/is_prime_u64.h"

static uint64_t *A231900_vals = NULL;
static size_t A231900_cnt = 0;
static size_t A231900_cap = 0;
static uint64_t A231900_k = 1;
static uint64_t A231900_D = 6;

static uint64_t A231900_gcd64(uint64_t x, uint64_t y)
{
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

static uint64_t A231900_rho(uint64_t n)
{
    if (n % 2 == 0) return 2;
    uint64_t c;
    for (c = 1; ; c++) {
        uint64_t x = 2, y = 2, d = 1;
        while (d == 1) {
            x = mulmod(x, x, n); x = (x + c) % n;
            y = mulmod(y, y, n); y = (y + c) % n;
            y = mulmod(y, y, n); y = (y + c) % n;
            d = A231900_gcd64(x > y ? x - y : y - x, n);
        }
        if (d != n) return d;
    }
}

static void A231900_add(uint64_t p, uint64_t *ps, int *cnt)
{
    int i;
    for (i = 0; i < *cnt; i++) if (ps[i] == p) return;
    ps[(*cnt)++] = p;
}

static void A231900_collect(uint64_t n, uint64_t *ps, int *cnt)
{
    if (n == 1) return;
    if (is_prime_u64(n)) { A231900_add(n, ps, cnt); return; }
    uint64_t d = A231900_rho(n);
    A231900_collect(d, ps, cnt);
    A231900_collect(n / d, ps, cnt);
}

static void A231900_step(void)
{
    uint64_t ps[128];
    int cnt = 0;
    uint64_t D = A231900_D, r = D, p;
    for (p = 2; p < 1000 && p * p <= r; p++) {
        if (r % p == 0) {
            A231900_add(p, ps, &cnt);
            while (r % p == 0) r /= p;
        }
    }
    if (r > 1) A231900_collect(r, ps, &cnt);
    uint64_t k = A231900_k, best = 0;
    int i;
    for (i = 0; i < cnt; i++) {
        uint64_t m = ps[i] * (k / ps[i] + 1);
        if (best == 0 || m < best) best = m;
    }
    uint64_t g = A231900_gcd64(D, best);
    A231900_k = best;
    A231900_D = D + g - 1;
    if (A231900_cnt >= A231900_cap) {
        size_t nc = A231900_cap ? A231900_cap * 2 : 64;
        uint64_t *t = (uint64_t *)realloc(A231900_vals, nc * sizeof(uint64_t));
        if (!t) exit(1);
        A231900_vals = t;
        A231900_cap = nc;
    }
    A231900_vals[A231900_cnt++] = g;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    while ((idx_t)A231900_cnt < n) A231900_step();
    return (seq_t)A231900_vals[n - 1];
}
