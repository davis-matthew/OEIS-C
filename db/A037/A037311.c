/*
 * A037311  Numbers whose base-3 and base-6 expansions have the same digit
 *          sum.
 * https://oeis.org/A037311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th positive integer whose base‑3 and base‑6 digit
 *            sums are equal
 * Range:     u8 n <= 32, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t last_checked = 0;

/* digit sum of v in the given base (base is 3 or 6) */
static unsigned int digit_sum_base(seq_t v, unsigned int base)
{
    unsigned int sum = 0;
    seq_t t = v;
    while (t > 0)
    {
        sum += (unsigned int)(t % base);
        t /= base;
    }
    return sum;
}

/* reallocate cache array; works in C and C++ */
static seq_t *realloc_cache(seq_t *ptr, size_t old_cap, size_t new_cap)
{
#ifdef __cplusplus
    seq_t *new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap && i < new_cap; ++i)
        new_ptr[i] = ptr[i];
    delete[] ptr;
    return new_ptr;
#else
    return (seq_t *)realloc(ptr, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1, so a(1) is first term */
    if ((size_t)n <= cache_len)     /* already computed */
        return cache[n - 1];

    /* ensure enough capacity */
    size_t needed = (size_t)n;
    if (cache_cap < needed)
    {
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap;
        while (new_cap < needed) new_cap <<= 1;
        cache = realloc_cache(cache, cache_cap, new_cap);
        if (!cache) abort();
        cache_cap = new_cap;
    }

    /* extend sequence until we have at least n terms */
    seq_t cand = last_checked + 1;
    while (cache_len < n)
    {
        if (digit_sum_base(cand, 3) == digit_sum_base(cand, 6))
            cache[cache_len++] = cand;
        ++cand;
    }
    last_checked = cand - 1;

    return cache[n - 1];
}
