/*
 * A117709  Pentagonal numbers for which the sum of the digits is also a
 *          pentagonal number.
 * https://oeis.org/A117709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 24, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   digit_sum, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for pentagonal numbers whose digit sum is pentagonal */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/floor_sqrt.h"

#ifdef __cplusplus
/* ---------- C++ implementation using std::vector ---------- */
#include <vector>

static std::vector<seq_t> cache;
static idx_t next_k = 0;

/* Test for a pentagonal number using the 24n+1 square test. */
static inline bool is_pentagonal(seq_t v)
{
    if (v == 0) return true;                 /* P(0) = 0 */
    seq_t t = (seq_t)24 * v + (seq_t)1;
    seq_t s = floor_sqrt(t);
    return (s * s == t) && ((s + (seq_t)1) % (seq_t)6 == 0);
}

/* Grow the cache until it contains at least (n+1) terms. */
static void generate_up_to(idx_t n)
{
    while (cache.size() <= n)
    {
        seq_t k   = (seq_t)next_k;
        seq_t pent = k * ((seq_t)3 * k - (seq_t)1) / (seq_t)2;  /* P(k) */
        seq_t ds   = digit_sum(pent);
        if (is_pentagonal(ds))
            cache.push_back(pent);
        ++next_k;
    }
}
#else
/* ---------- C implementation using manual allocation ---------- */
static seq_t *cache = NULL;
static size_t cap   = 0;
static idx_t len    = 0;
static idx_t next_k = 0;

/* Test for a pentagonal number using the 24n+1 square test. */
static inline bool is_pentagonal(seq_t v)
{
    if (v == 0) return true;                 /* P(0) = 0 */
    seq_t t = (seq_t)24 * v + (seq_t)1;
    seq_t s = floor_sqrt(t);
    return (s * s == t) && ((s + (seq_t)1) % (seq_t)6 == 0);
}

/* Ensure capacity for at least one more element. */
static void cache_grow(void)
{
    if (len >= cap)
    {
        size_t new_cap = (cap == 0) ? 16 : cap * 2;
        void *p = realloc(cache, new_cap * sizeof(seq_t));
        if (!p) abort();
        cache = (seq_t *)p;
        cap   = new_cap;
    }
}

/* Grow the cache until it holds at least (n+1) terms. */
static void generate_up_to(idx_t n)
{
    while (len <= n)
    {
        seq_t k   = (seq_t)next_k;
        seq_t pent = k * ((seq_t)3 * k - (seq_t)1) / (seq_t)2;  /* P(k) */
        seq_t ds   = digit_sum(pent);
        if (is_pentagonal(ds))
        {
            cache_grow();
            cache[len++] = pent;
        }
        ++next_k;
    }
}
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    generate_up_to(n);
    return cache[(size_t)n];
#else
    if (!cache)
    {
        cache = (seq_t *)malloc(16 * sizeof(seq_t));
        if (!cache) abort();
        cap = 16;
        len = 0;
        next_k = 0;
    }
    generate_up_to(n);
    return cache[n];
#endif
}
