/*
 * A105212  a(1) = 668; for n > 1, a(n) = a(n-1) + 1 + sum of distinct prime
 *          factors of a(n-1) that are < a(n-1).
 * https://oeis.org/A105212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 668; for n>1, a(n) = a(n-1) + 1 + sum of distinct prime
 *            factors of a(n-1) that are < a(n-1)
 * Range:     u8 none, u16 n <= 78, u32 n <= 532, u64 n <= 1877,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 56 terms through n = 1741 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 668; a(n) = a(n-1) + 1 + the sum of the distinct prime factors
 * of a(n-1) below a(n-1) (0 if it is prime). The terms pass 2^64 (about 3e20
 * by n = 2000), so they are factored in unsigned __int128: trial division by
 * the small primes, then for what is left an exact Miller-Rabin test (the
 * first 13 prime bases decide every number below 3.3e24) and Brent's
 * Pollard rho. mulmod splits one factor into 35-bit halves so that every
 * product fits in 128 bits (moduli below 2^81; the terms here stay below
 * 2^69). (The original did trial
 * division to sqrt(a(n)) in seq_t, which cannot finish at these sizes.)
 * Widths narrower than the terms wrap, as before; big is exact. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

typedef unsigned __int128 u128;

static u128 mulmod(u128 a, u128 b, u128 m)          /* a, b < m < 2^81 */
{
    u128 b1 = b >> 35, b0 = b & (((u128)1 << 35) - 1);
    u128 hi = (a * b1) % m;                           /* < 2^81 * 2^46 */
    hi = ((hi << 35) % m + (a * b0) % m) % m;
    return hi;
}

static u128 powmod(u128 b, u128 e, u128 m)
{
    u128 r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = mulmod(r, b, m);
        b = mulmod(b, b, m);
        e >>= 1;
    }
    return r;
}

static bool is_prime128(u128 n)
{
    static const unsigned P[13] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41};
    if (n < 2) return false;
    for (int i = 0; i < 13; ++i)
        if (n % P[i] == 0) return n == P[i];
    u128 d = n - 1;
    int s = 0;
    while (!(d & 1)) { d >>= 1; ++s; }
    for (int i = 0; i < 13; ++i) {
        u128 x = powmod(P[i], d, n);
        if (x == 1 || x == n - 1) continue;
        bool ok = false;
        for (int r = 1; r < s; ++r) {
            x = mulmod(x, x, n);
            if (x == n - 1) { ok = true; break; }
        }
        if (!ok) return false;
    }
    return true;
}

static u128 gcd128(u128 a, u128 b)
{
    while (b) { u128 t = a % b; a = b; b = t; }
    return a;
}

static u128 rho(u128 n)                               /* a nontrivial factor of composite n */
{
    if (!(n & 1)) return 2;
    for (u128 c = 1;; ++c) {
        u128 y = 2, x = 2, g = 1, q = 1, ys = 2;
        uint64_t r = 1, m = 128;
        do {
            x = y;
            for (uint64_t i = 0; i < r; ++i) y = (mulmod(y, y, n) + c) % n;
            uint64_t k = 0;
            do {
                ys = y;
                for (uint64_t i = 0; i < m && i < r - k; ++i) {
                    y = (mulmod(y, y, n) + c) % n;
                    q = mulmod(q, x > y ? x - y : y - x, n);
                }
                g = gcd128(q, n);
                k += m;
            } while (k < r && g == 1);
            r *= 2;
        } while (g == 1);
        if (g == n) {
            do {
                ys = (mulmod(ys, ys, n) + c) % n;
                g = gcd128(x > ys ? x - ys : ys - x, n);
            } while (g == 1);
        }
        if (g != n) return g;
    }
}

/* add the distinct prime factors of n to ps[0..*np) */
static void factor_into(u128 n, u128 *ps, int *np)
{
    if (n == 1) return;
    if (is_prime128(n)) {
        for (int i = 0; i < *np; ++i)
            if (ps[i] == n) return;
        ps[(*np)++] = n;
        return;
    }
    u128 d = rho(n);
    factor_into(d, ps, np);
    factor_into(n / d, ps, np);
}

static u128 sopf_below(u128 v)
{
    u128 ps[128], m = v, sum = 0;
    int np = 0;
    for (uint64_t p = 2; p < 10000 && (u128)p * p <= m; ++p)
        if (m % p == 0) {
            ps[np++] = p;
            while (m % p == 0) m /= p;
        }
    factor_into(m, ps, &np);
    for (int i = 0; i < np; ++i)
        if (ps[i] < v) sum += ps[i];
    return sum;
}

seq_t a(idx_t n)
{
    static seq_t *A = NULL;
    static u128 last = 0;                             /* a(len) exactly */
    static size_t len = 0, cap = 0;
    if (n == 0) return (seq_t)0;
    while (len < (size_t)n) {
        u128 v = len ? last + 1 + sopf_below(last) : (u128)668;
        if (len == cap) {
            cap = cap ? 2 * cap : 64;
            seq_t *t = mem_resize(seq_t, A, cap);
            if (!t) abort();
            A = t;
        }
        seq_t hi = (seq_t)(uint64_t)(v >> 64), lo = (seq_t)(uint64_t)v;
        A[len++] = sizeof(seq_t) > 8 ? ((hi << 32) << 32) + lo : lo;
        last = v;
    }
    return A[n - 1];
}
