/*
 * A235866  G-cyclic numbers: numbers n such that gcd(n,A060968(n))=1.
 * https://oeis.org/A235866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m (m>=1) with gcd(m, A060968(m)) = 1; found by an
 *            incremental search with cached terms
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9732 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

static uint64_t A235866_gcd(uint64_t x, uint64_t y)
{
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

static bool A235866_good(uint64_t m)
{
    uint64_t r = m, A = 1;
    if ((m & 1ULL) == 0) return false; /* A060968(m) is even for even m */
    for (uint64_t p = 3; p * p <= r; p += 2) {
        if (r % p == 0) {
            uint64_t part = 1;
            while (r % p == 0) { r /= p; part *= p; }
            part /= p;
            part *= ((p & 3ULL) == 1ULL) ? (p - 1) : (p + 1);
            A *= part;
        }
    }
    if (r > 1) A *= ((r & 3ULL) == 1ULL) ? (r - 1) : (r + 1);
    return A235866_gcd(m, A) == 1;
}

static uint64_t *A235866_cache = NULL;
static size_t A235866_cnt = 0, A235866_cap = 0;
static uint64_t A235866_last = 0;

seq_t a(idx_t n)
{
    while (A235866_cnt < (size_t)n) {
        ++A235866_last;
        if (A235866_good(A235866_last)) {
            if (A235866_cnt == A235866_cap) {
                A235866_cap = A235866_cap ? A235866_cap * 2 : 1024;
                A235866_cache = (uint64_t *)realloc(A235866_cache, A235866_cap * sizeof(uint64_t));
            }
            A235866_cache[A235866_cnt++] = A235866_last;
        }
    }
    return (seq_t)A235866_cache[(size_t)n - 1];
}
