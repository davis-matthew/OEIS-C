/*
 * A064634  a(0) = 0 and a(n) = Min { i > a(n-1) | A064466(i + 1) - A064466(i)
 *          > 2 } for n > 0.
 * https://oeis.org/A064634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(n) = Min { i > a(n-1) | A064466(i+1) - A064466(i) >
 *            2 }; computed by search over a memoised A064466
 * Range:     u8 n <= 43, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 54 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

/* ---- A064466: a(0)=6, a(n)=min{m>a(n-1) | a(n-1)-p and m-p both prime for some prime p} ---- */
static uint64_t *A064466_cache = NULL;
static size_t A064466_len = 0, A064466_cap = 0;

static uint64_t A064466_next_prime(uint64_t n)
{
    uint64_t c = n + 1;
    if (c <= 2) return 2;
    if ((c & 1) == 0) c++;
    while (!is_prime_u64(c)) c += 2;
    return c;
}

static uint64_t A064466_step(uint64_t prev)
{
    uint64_t best = 0;
    uint64_t p;
    for (p = 2; p + 2 <= prev; p = A064466_next_prime(p)) {
        uint64_t q = prev - p;
        if (!is_prime_u64(q)) continue;
        uint64_t m = p + A064466_next_prime(q);
        if (best == 0 || m < best) best = m;
    }
    return best ? best : prev + 2;
}

static uint64_t A064466_get(size_t k)
{
    if (k >= A064466_cap) {
        size_t nc = A064466_cap ? A064466_cap : 64;
        while (nc <= k) nc <<= 1;
        uint64_t *t = (uint64_t *)realloc(A064466_cache, nc * sizeof(uint64_t));
        A064466_cache = t;
        A064466_cap = nc;
    }
    if (A064466_len == 0) {
        A064466_cache[0] = 6;
        A064466_len = 1;
    }
    while (A064466_len <= k) {
        A064466_cache[A064466_len] = A064466_step(A064466_cache[A064466_len - 1]);
        A064466_len++;
    }
    return A064466_cache[k];
}

/* ---- A064634 ---- */
static uint64_t *res_cache = NULL;
static size_t res_len = 0, res_cap = 0;

seq_t a(idx_t n)
{
    size_t k = (size_t)n;
    if (k >= res_cap) {
        size_t nc = res_cap ? res_cap : 64;
        while (nc <= k) nc <<= 1;
        uint64_t *t = (uint64_t *)realloc(res_cache, nc * sizeof(uint64_t));
        res_cache = t;
        res_cap = nc;
    }
    if (res_len == 0) {
        res_cache[0] = 0;
        res_len = 1;
    }
    while (res_len <= k) {
        size_t i = (size_t)res_cache[res_len - 1] + 1;
        while (A064466_get(i + 1) - A064466_get(i) <= 2) i++;
        res_cache[res_len++] = (uint64_t)i;
    }
    return (seq_t)res_cache[k];
}
