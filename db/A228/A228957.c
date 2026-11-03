/*
 * A228957  Numbers n such that n/rad(n) is greater than the greatest prime
 *          dividing n.
 * https://oeis.org/A228957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 864, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9874 (2026-11-03)
 * Helpers:   build_primes_upto, ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers n such that n/rad(n) is greater than the greatest prime factor of n */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

static seq_t *cache   = NULL;
static size_t cache_len = 0, cache_cap = 0;

/* Prime table used for factorisation */
static uint64_t *primes   = NULL;
static size_t    prime_cnt = 0;
static idx_t     prime_limit = 0;

static void ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_limit) return;
    free(primes);
    build_primes_upto(limit, &primes, &prime_cnt);
    prime_limit = limit;
}

static bool is_good(seq_t x)
{
    /* 1 and numbers that do not fit in 64 bits are ignored */
    if (x <= (seq_t)1) return false;
    if (x > (seq_t)UINT64_MAX) return false;

    idx_t n      = (idx_t)x;
    idx_t temp   = n;
    seq_t rad    = 1;
    uint64_t gpf = 0;

    seq_t sq_t = floor_sqrt(x);          /* sqrt(x) as seq_t */
    idx_t sqrtx = (idx_t)sq_t;

    ensure_primes_upto(sqrtx);

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > (uint64_t)sqrtx) break;
        if (temp % (idx_t)p == 0) {
            rad *= (seq_t)p;
            gpf = p;
            while (temp % (idx_t)p == 0) temp /= (idx_t)p;
        }
    }
    if (temp > 1) {
        rad *= (seq_t)temp;
        if ((uint64_t)temp > gpf) gpf = temp;
    }

    seq_t quot = x / rad;
    return quot > (seq_t)gpf;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined, but harmless */

    if (n <= cache_len) return cache[n-1];

    seq_t cur = (cache_len > 0) ? cache[cache_len-1] + 1 : 1;
    for (;; ++cur) {
        if (is_good(cur)) {
            append_candidate(&cache, &cache_len, &cache_cap, cur);
            if (cache_len == n) return cur;
        }
    }
}
