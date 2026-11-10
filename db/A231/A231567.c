/*
 * A231567  Numbers n such that A231566(n) <> n.
 * https://oeis.org/A231567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number k such that A231566(k) != k, where A231566(k)
 *            = sum_{i=1}^{2k} gcd(i,2k)*i^(2k) mod 2k; found by incremental
 *            search with memoization
 * Range:     u8 n <= 46, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static uint64_t A231567_gcd(uint64_t x, uint64_t y)
{
    while (y) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

static uint64_t A231567_powmod(uint64_t b, uint64_t e, uint64_t m)
{
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = (r * b) % m;
        b = (b * b) % m;
        e >>= 1;
    }
    return r;
}

static uint64_t A231567_A231566(uint64_t n)
{
    uint64_t m = 2 * n;
    uint64_t sum = 0;
    for (uint64_t i = 1; i <= m; ++i) {
        uint64_t g = A231567_gcd(i, m);
        uint64_t p = A231567_powmod(i, m, m);
        sum = (sum + (g * p) % m) % m;
    }
    return sum;
}

static uint64_t *A231567_terms = 0;
static size_t A231567_count = 0;
static size_t A231567_cap = 0;
static uint64_t A231567_next = 1;

seq_t a(idx_t n)
{
    while (A231567_count < (size_t)n) {
        uint64_t k = A231567_next++;
        if (A231567_A231566(k) != k) {
            if (A231567_count == A231567_cap) {
                size_t nc = A231567_cap ? A231567_cap * 2 : 256;
                A231567_terms = (uint64_t *)realloc(A231567_terms, nc * sizeof(uint64_t));
                A231567_cap = nc;
            }
            A231567_terms[A231567_count++] = k;
        }
    }
    return (seq_t)A231567_terms[(size_t)n - 1];
}
