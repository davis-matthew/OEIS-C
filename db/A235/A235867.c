/*
 * A235867  G-cyclic numbers k such that A060968(k)^A060968(k) <> 1 (mod k)
 *          and A235863(k)^A235863(k) <> 1 (mod k).
 * https://oeis.org/A235867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th composite k with gcd(k, A060968(k)) = 1 (G-cyclic)
 *            such that A060968(k)^A060968(k) != 1 (mod k) and
 *            A235863(k)^A235863(k) != 1 (mod k); found by a cached search
 * Range:     u8 n <= 6, u16 n <= 7034, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9785 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t g235867_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static uint64_t g235867_powmod(uint64_t b, uint64_t e, uint64_t m)
{
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = (uint64_t)(((unsigned __int128)r * b) % m);
        b = (uint64_t)(((unsigned __int128)b * b) % m);
        e >>= 1;
    }
    return r;
}

static bool g235867_test(uint64_t k)
{
    if (k < 4) return false;
    uint64_t ord = 1, ex = 1, rem = k;
    for (uint64_t p = 2; p * p <= rem || rem > 1; ) {
        uint64_t q;
        unsigned e = 0;
        if (p * p > rem) { q = rem; e = 1; rem = 1; }
        else {
            if (rem % p) { p += (p == 2) ? 1 : 2; continue; }
            q = p;
            while (rem % p == 0) { rem /= p; ++e; }
        }
        uint64_t po, pe;
        if (q == 2) {
            if (e == 1) { po = 2; pe = 2; }
            else {
                po = (uint64_t)1 << (e + 1);
                pe = (e <= 4) ? 4 : ((uint64_t)1 << (e - 2));
            }
        } else {
            uint64_t pp = 1;
            for (unsigned j = 1; j < e; ++j) pp *= q;
            po = ((q & 3) == 1 ? q - 1 : q + 1) * pp;
            pe = po;
        }
        ord *= po;
        ex = (ex / g235867_gcd(ex, pe)) * pe;
        if (rem == 1) break;
        if (q == p) p += (p == 2) ? 1 : 2;
    }
    if (g235867_gcd(k, ord) != 1) return false;
    if (g235867_powmod(ord, ord, k) == 1) return false;
    if (g235867_powmod(ex, ex, k) == 1) return false;
    return true;
}

static idx_t *g235867_terms = 0;
static size_t g235867_cnt = 0, g235867_cap = 0;
static uint64_t g235867_next = 4;

seq_t a(idx_t n)
{
    while (g235867_cnt < (size_t)n) {
        if (g235867_test(g235867_next)) {
            if (g235867_cnt == g235867_cap) {
                g235867_cap = g235867_cap ? g235867_cap * 2 : 1024;
                g235867_terms = (idx_t *)realloc(g235867_terms, g235867_cap * sizeof(idx_t));
            }
            g235867_terms[g235867_cnt++] = (idx_t)g235867_next;
        }
        ++g235867_next;
    }
    return (seq_t)g235867_terms[n - 1];
}
