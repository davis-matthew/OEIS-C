/*
 * A197680  Numbers whose exponents in their prime power factorization are
 *          squares.
 * https://oeis.org/A197680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 166, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_square_exponent
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by enumerating candidates and checking that all prime
   exponents in the factorisation are perfect squares. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_square_exponent.h"

static seq_t *cache = NULL;      /* cached a(k) values */
static size_t cache_len = 0, cache_cap = 0;
static uint64_t cur_val = 0;     /* last integer tested */

/* Prime list for sieving */
static uint64_t *primes = NULL;
static size_t primes_len = 0, primes_cap = 0;
static uint64_t primes_max = 0;

/* Integer square‑root for 64‑bit numbers. */
static inline uint64_t uint64_isqrt(uint64_t n)
{
    uint64_t lo = 0, hi = ((uint64_t)1 << 32);
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n) {
            if ((__uint128_t)(mid + 1) * (mid + 1) > n) return mid;
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

/* Build prime table up to 'limit' inclusively.  Reallocs only when needed. */
static void build_primes_upto(uint64_t limit)
{
    if (limit <= primes_max) return;          /* already built */
    size_t sz = limit + 1;
    bool *is_comp = (bool *)calloc(sz, sizeof(bool));
    if (!is_comp) return;                    /* allocation failure aborted */

    for (uint64_t i = 2; i * i <= limit; ++i)
        if (!is_comp[i])
            for (uint64_t j = i * i; j <= limit; j += i)
                is_comp[j] = true;

    size_t new_len = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) ++new_len;

    if (primes_cap < new_len) {
        void *tmp = realloc(primes, new_len * sizeof(uint64_t));
        primes = (uint64_t *)tmp;
        primes_cap = new_len;
    }

    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) primes[idx++] = i;

    primes_len = new_len;
    primes_max = limit;
    free(is_comp);
}

/* Return true iff all prime exponents in x are perfect squares. */
static bool good_number(uint64_t x)
{
    if (x == 1) return true;    /* 1 has empty factorisation */

    uint64_t limit = uint64_isqrt(x);
    build_primes_upto(limit);

    uint64_t tmp = x;
    for (size_t i = 0; i < primes_len && primes[i] * primes[i] <= tmp; ++i) {
        uint64_t p = primes[i];
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            if (!is_square_exponent(e)) return false;
        }
    }
    /* Remaining prime factor (if any) has exponent 1 = 1², OK. */
    return true;
}

/* Append a value to the cache, growing storage if necessary. */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        void *tmp = realloc(cache, new_cap * sizeof(seq_t));
        cache = (seq_t *)tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Public function ------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;            /* undefined for 0 */

    /* Initialise cache with first term if empty. */
    if (cache_len == 0) {
        cache_append((seq_t)1);
        cur_val = 1;
    }

    /* Generate further terms until we have at least n entries. */
    while (cache_len < (size_t)n) {
        ++cur_val;
        if (good_number(cur_val))
            cache_append((seq_t)cur_val);
    }

    return cache[(size_t)(n - 1)];          /* a(n) is the (n-1)-th element */
}
