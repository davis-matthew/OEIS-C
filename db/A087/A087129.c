/*
 * A087129  First differences of A087128.
 * https://oeis.org/A087129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A087128(n+1) - A087128(n), where A087128(1) = 1 and
 *            A087128(n) is found by searching for the smallest t >
 *            A087128(n-1) such that 1 + Sum_{k=A087128(n-1)+1..t} k is prime
 * Range:     u8 n <= 2621, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 98 terms through n = 9825 (2026-11-07)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

static uint64_t *A087128_cache = NULL;
static idx_t A087128_len = 0;
static idx_t A087128_cap = 0;

static void A087128_grow(idx_t needed)
{
    if (needed <= A087128_cap)
        return;
    idx_t new_cap = A087128_cap ? A087128_cap << 1 : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    A087128_cache = (uint64_t *)realloc(A087128_cache, (size_t)new_cap * sizeof(uint64_t));
    A087128_cap = new_cap;
}

static uint64_t A087128_next(uint64_t prev)
{
    uint64_t sum = 0;
    uint64_t t = prev + 1;
    for (;; ++t) {
        sum += t;
        if (is_prime((idx_t)(sum + 1)))
            return t;
    }
}

static void A087128_fill(idx_t n)
{
    if (A087128_len == 0) {
        A087128_grow(1);
        A087128_cache[0] = 1;
        A087128_len = 1;
    }
    if (A087128_len < n) {
        A087128_grow(n);
        for (idx_t i = A087128_len; i < n; ++i)
            A087128_cache[i] = A087128_next(A087128_cache[i - 1]);
        A087128_len = n;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    A087128_fill(n + 1);
    return (seq_t)(A087128_cache[n] - A087128_cache[n - 1]);
}
