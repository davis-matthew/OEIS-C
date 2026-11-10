/*
 * A069802  Primes related to the nondecreasing subsequence of A053666.
 * https://oeis.org/A069802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search. a(n) = n-th prime p, in increasing order, whose digit
 *            product A053666 is >= that of the previously kept prime (the
 *            nondecreasing subsequence of A053666), starting with a(1) = 2.
 *            Candidates with too small a digit product are skipped
 *            digit-wise, and primality is checked with deterministic M...
 * Range:     u8 n <= 13, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t A069802_digprod(uint64_t x)
{
    uint64_t p = 1;
    while (x) { p *= (x % 10); x /= 10; }
    return p;
}

static uint64_t A069802_mulmod(uint64_t a, uint64_t b, uint64_t m)
{
    return (uint64_t)(((unsigned __int128)a * b) % m);
}

static uint64_t A069802_powmod(uint64_t b, uint64_t e, uint64_t m)
{
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = A069802_mulmod(r, b, m);
        b = A069802_mulmod(b, b, m);
        e >>= 1;
    }
    return r;
}

static bool A069802_isprime(uint64_t n)
{
    static const uint64_t small[12] = {2,3,5,7,11,13,17,19,23,29,31,37};
    int i;
    if (n < 2) return false;
    for (i = 0; i < 12; ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    for (i = 0; i < 12; ++i) {
        uint64_t x = A069802_powmod(small[i], d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        int r;
        for (r = 1; r < s; ++r) {
            x = A069802_mulmod(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* DFS over digits: smallest L-digit number >= xd (if tight) whose digit
   product is >= P.  Returns 1 on success, digits in out. */
static int A069802_dfs(const int *xd, int L, int i, bool tight,
                       uint64_t prod, uint64_t P, int *out,
                       const uint64_t *pw9)
{
    if (i == L) return prod >= P;
    int start = tight ? xd[i] : 1;
    if (start < 1) start = 1;
    int d;
    for (d = start; d <= 9; ++d) {
        uint64_t np = prod * (uint64_t)d;
        if (np * pw9[L - i - 1] < P) continue;
        out[i] = d;
        if (A069802_dfs(xd, L, i + 1, tight && d == xd[i], np, P, out, pw9))
            return 1;
    }
    return 0;
}

/* smallest y >= x with digit product >= P */
static uint64_t A069802_next_ok(uint64_t x, uint64_t P)
{
    uint64_t pw9[20];
    int xd[20], out[20];
    int k, L;
    if (P == 0) return x;
    pw9[0] = 1;
    for (k = 1; k < 20; ++k) pw9[k] = pw9[k - 1] * 9;
    L = 0;
    { uint64_t t = x; while (t) { ++L; t /= 10; } }
    if (L == 0) L = 1;
    { uint64_t t = x; for (k = L - 1; k >= 0; --k) { xd[k] = (int)(t % 10); t /= 10; } }
    bool tight = true;
    while (L <= 19) {
        if (A069802_dfs(xd, L, 0, tight, 1, P, out, pw9)) {
            uint64_t y = 0;
            for (k = 0; k < L; ++k) y = y * 10 + (uint64_t)out[k];
            return y;
        }
        ++L;
        tight = false;
    }
    abort();
    return 0;
}

seq_t a(idx_t n)
{
    static uint64_t *kept = NULL;
    static size_t cnt = 0, cap = 0;
    static uint64_t cand = 2;      /* next candidate to examine */
    static uint64_t lastprod = 0;

    if (n < 1) return 0;
    while (cnt < (size_t)n) {
        uint64_t c = A069802_next_ok(cand, lastprod);
        if (A069802_isprime(c)) {
            if (cnt == cap) {
                cap = cap ? cap * 2 : 64;
                kept = (uint64_t *)realloc(kept, cap * sizeof(uint64_t));
                if (!kept) abort();
            }
            kept[cnt++] = c;
            lastprod = A069802_digprod(c);
        }
        cand = c + 1;
    }
    return (seq_t)kept[(size_t)n - 1];
}
