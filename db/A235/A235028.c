/*
 * A235028  Fixed points of A235027.
 * https://oeis.org/A235028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th k >= 0 with A235027(k) = k, where A235027 is
 *            completely multiplicative with A235027(2) = 2 and A235027(p) =
 *            binary reversal of p for odd primes p; found by searching k in
 *            increasing order
 * Range:     u8 n <= 106, u16 n <= 1897, u32 n <= 1897, u64 n <= 1897,
 *            u128 n <= 1897, big any n
 * Validated: standard, pass, 81 terms through n = 1855 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t A235028_rev(uint64_t x)
{
    uint64_t r = 0;
    while (x) { r = (r << 1) | (x & 1); x >>= 1; }
    return r;
}

/* tests A235027(k) == k, k >= 2 */
static bool A235028_fixed(uint64_t k)
{
    uint64_t t = k, res = 1;
    while ((t & 1) == 0) { t >>= 1; res *= 2; }
    for (uint64_t d = 3; d * d <= t; d += 2) {
        while (t % d == 0) {
            t /= d;
            res *= A235028_rev(d);
            if (res > k) return false;
        }
    }
    if (t > 1) res *= A235028_rev(t);
    return res == k;
}

static uint64_t *A235028_cache = NULL;
static size_t A235028_cnt = 0, A235028_cap = 0;
static uint64_t A235028_last = 1;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (A235028_cnt == 0) {
        A235028_cap = 64;
        A235028_cache = (uint64_t *)malloc(A235028_cap * sizeof(uint64_t));
        A235028_cache[0] = 0;
        A235028_cache[1] = 1;
        A235028_cnt = 2;
        A235028_last = 1;
    }
    while (A235028_cnt < (size_t)n) {
        uint64_t k = A235028_last + 1;
        while (!A235028_fixed(k)) ++k;
        A235028_last = k;
        if (A235028_cnt >= A235028_cap) {
            A235028_cap *= 2;
            A235028_cache = (uint64_t *)realloc(A235028_cache, A235028_cap * sizeof(uint64_t));
        }
        A235028_cache[A235028_cnt++] = k;
    }
    return (seq_t)A235028_cache[n - 1];
}
