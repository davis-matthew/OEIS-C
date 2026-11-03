/*
 * A048132  Becomes prime or 4 after exactly 10 iterations of f(x) = sum of
 *          prime factors of x.
 * https://oeis.org/A048132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer that becomes a prime or 4 after exactly
 *            10 iterations of f(x) = sum of prime factors (with multiplicity)
 * Range:     u8 none, u16 n <= 7, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 32 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static idx_t *spf = NULL;        /* smallest prime factor array */
static idx_t spf_limit = 0;      /* current size of spf */

static void ensure_limit(idx_t n)
{
    if (n <= spf_limit) return;

    idx_t new_limit = spf_limit;
    if (new_limit == 0) new_limit = 1;
    while (new_limit < n) new_limit <<= 1;

    idx_t *new_spf = (idx_t*)malloc((new_limit + 1) * sizeof(idx_t));
    for (idx_t i = 0; i <= new_limit; ++i) new_spf[i] = i;
    for (idx_t i = 2; i <= new_limit / i; ++i)
    {
        if (new_spf[i] == i)
        {
            for (idx_t j = i * i; j <= new_limit; j += i)
                if (new_spf[j] == j) new_spf[j] = i;
        }
    }
    free(spf);
    spf = new_spf;
    spf_limit = new_limit;
}

static inline bool is_prime(idx_t x)
{
    return x >= 2 && spf[x] == x;
}

static inline uint64_t sum_prime_factors(idx_t x)
{
    uint64_t sum = 0;
    while (x > 1)
    {
        uint64_t p = spf[x];
        sum += p;
        x /= p;
    }
    return sum;
}

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t checked = 2;

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    while (cache_len < n)
    {
        if (checked > spf_limit) ensure_limit(checked);

        uint64_t t = checked;
        bool fail = false;

        for (int step = 1; step <= 10; ++step)
        {
            t = sum_prime_factors((idx_t)t);
            if (step < 10 && (t == 4 || is_prime((idx_t)t)))
            {
                fail = true;
                break;
            }
        }

        if (!fail && (t == 4 || is_prime((idx_t)t)))
        {
            if (cache_len + 1 > cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                if (new_cap < cache_len + 1) new_cap = cache_len + 1;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)checked;
        }

        ++checked;
    }

    return cache[n - 1];
}
