/*
 * A231565  Numbers such that A231563(n)>0.
 * https://oeis.org/A231565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = n-th m >= 1 with A231563(m) > 0, where A231563(m)
 *            = (Sum_{1<=i<=m, gcd(i,m)=1} i^m) mod m
 * Range:     u8 n <= 52, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t A231565_gcd(uint64_t x, uint64_t y)
{
    while (y) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

static uint64_t A231565_powmod(uint64_t b, uint64_t e, uint64_t m)
{
    unsigned __int128 r = 1 % m, x = b % m;
    while (e) {
        if (e & 1) r = (r * x) % m;
        x = (x * x) % m;
        e >>= 1;
    }
    return (uint64_t)r;
}

static bool A231565_positive(uint64_t m)
{
    uint64_t sum = 0;
    for (uint64_t i = 1; i <= m; ++i) {
        if (A231565_gcd(i, m) == 1) {
            sum += A231565_powmod(i, m, m);
            if (sum >= m)
                sum -= m;
        }
    }
    return (sum % m) != 0;
}

static uint64_t *A231565_terms = 0;
static size_t A231565_cnt = 0;
static size_t A231565_cap = 0;
static uint64_t A231565_next = 1;

seq_t a(idx_t n)
{
    while (A231565_cnt < (size_t)n) {
        uint64_t m = A231565_next++;
        if (A231565_positive(m)) {
            if (A231565_cnt == A231565_cap) {
                size_t nc = A231565_cap ? A231565_cap * 2 : 256;
                uint64_t *p = (uint64_t *)realloc(A231565_terms, nc * sizeof(uint64_t));
                if (!p) return 0;
                A231565_terms = p;
                A231565_cap = nc;
            }
            A231565_terms[A231565_cnt++] = m;
        }
    }
    return (seq_t)A231565_terms[(size_t)n - 1];
}
