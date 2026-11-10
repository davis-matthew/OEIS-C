/*
 * A235044  Partial sums of the characteristic function of A091214.
 * https://oeis.org/A235044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k <= n with k composite and the binary
 *            expansion of k an irreducible polynomial over GF(2) (partial
 *            sums of the characteristic function of A091214); computed by
 *            sieve-like scan with memoized prefix counts
 * Range:     u8 n <= 4118, u16 n <= 4119, u32 n <= 4119, u64 n <= 4119,
 *            u128 n <= 4119, big any n
 * Validated: standard, pass, 108 terms through n = 3896 (2026-11-07)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static int A235044_deg(uint64_t x)
{
    int d = -1;
    while (x) { d++; x >>= 1; }
    return d;
}

static uint64_t A235044_pmod(uint64_t a, uint64_t b)
{
    int db = A235044_deg(b);
    int da;
    while (a && (da = A235044_deg(a)) >= db) a ^= b << (da - db);
    return a;
}

static uint64_t A235044_pgcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t r = A235044_pmod(a, b); a = b; b = r; }
    return a;
}

static uint64_t A235044_pmulmod(uint64_t a, uint64_t b, uint64_t m)
{
    int dm = A235044_deg(m);
    uint64_t r = 0;
    for (int i = A235044_deg(b); i >= 0; --i) {
        r <<= 1;
        if ((r >> dm) & 1) r ^= m;
        if ((b >> i) & 1) r ^= a;
    }
    return r;
}

static bool A235044_irreducible(uint64_t m)
{
    int d = A235044_deg(m);
    if (d <= 1) return false;
    uint64_t g = 2;
    for (int i = 0; i < d / 2; ++i) {
        g = A235044_pmulmod(g, g, m);
        if (A235044_pgcd(m, g ^ 2) != 1) return false;
    }
    return true;
}

static uint64_t *A235044_cnt = NULL;
static size_t A235044_len = 0;   /* counts valid for indices 0..len-1 */
static size_t A235044_cap = 0;

seq_t a(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need > A235044_cap) {
        size_t nc = A235044_cap ? A235044_cap : 1024;
        while (nc < need) nc *= 2;
        uint64_t *t = (uint64_t *)realloc(A235044_cnt, nc * sizeof(uint64_t));
        if (!t) exit(EXIT_FAILURE);
        A235044_cnt = t;
        A235044_cap = nc;
    }
    while (A235044_len < need) {
        size_t k = A235044_len;
        uint64_t prev = k ? A235044_cnt[k - 1] : 0;
        if (k >= 4 && !is_prime((idx_t)k) && A235044_irreducible((uint64_t)k)) prev++;
        A235044_cnt[k] = prev;
        A235044_len++;
    }
    return (seq_t)A235044_cnt[(size_t)n];
}
