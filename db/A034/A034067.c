/*
 * A034067  Decimal part of a(n)^(1/2) starts with n so that a(n)<a(n+1).
 * https://oeis.org/A034067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 9, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   digits, floor_sqrt, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(0) = 26. For n>0, a(n) is the smallest integer > a(n‑1)
   such that the decimal part of sqrt(a(n)) begins with the decimal digits
   of n and a(n) is not a perfect square. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/digits.h"
#include "../utils/pow10.h"

static bool is_square(seq_t x)
{
    seq_t r = floor_sqrt(x);
    return r * r == x;
}

static seq_t compute_value(idx_t i, seq_t prev)
{
    int k = digits(i);
    seq_t pow10k = pow10((idx_t)k);
    seq_t pow10_2k = pow10((idx_t)(2 * k));

    seq_t s = floor_sqrt(prev + (seq_t)1);

    for (;; ++s)
    {
        seq_t base = s * pow10k;

        seq_t numL = base + (seq_t)i;
        seq_t numLsq = numL * numL;
        seq_t lower = (numLsq + pow10_2k - (seq_t)1) / pow10_2k;

        seq_t numU = base + (seq_t)i + (seq_t)1;
        seq_t numUsq = numU * numU;
        seq_t upper = (numUsq - (seq_t)1) / pow10_2k;

        seq_t candidate = lower > (prev + (seq_t)1) ? lower : (prev + (seq_t)1);
        if (candidate > upper)
            continue;

        while (candidate <= upper)
        {
            if (!is_square(candidate))
                return candidate;
            ++candidate;
        }
    }
}

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> cache_vec;
#else
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

static void ensure_capacity(idx_t n)
{
    if ((idx_t)cache_cap > n)
        return;
    size_t new_cap = (cache_cap ? cache_cap << 1 : 64);
    while ((idx_t)new_cap <= n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    if (n < cache_vec.size())
        return cache_vec[n];
    for (idx_t i = cache_vec.size(); i <= n; ++i)
    {
        seq_t prev = (i == 0 ? 0 : cache_vec[i - 1]);
        seq_t val = compute_value(i, prev);
        cache_vec.push_back(val);
    }
    return cache_vec[n];
#else
    if ((idx_t)cache_len > n)
        return cache[n];
    ensure_capacity(n);
    for (idx_t i = cache_len; i <= n; ++i)
    {
        seq_t prev = (i == 0 ? 0 : cache[i - 1]);
        cache[i] = compute_value(i, prev);
    }
    cache_len = n + 1;
    return cache[n];
#endif
}
