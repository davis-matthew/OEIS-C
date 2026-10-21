/*
 * A207472  Let a(1) = 5. For n > 1, a(n) is the least number greater than
 *          a(n-1) such that the Hamming distance D(a(n-1),a(n)) = 5.
 * https://oeis.org/A207472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 3334, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 5.
   For n > 1, a(n) = the least integer > a(n-1) such that
   popcount(a(n-1) XOR a(n)) = 5. */
#include <stdlib.h>
#include <stdint.h>

/* popcount for a generic unsigned integer type seq_t. */
static inline unsigned int popcount_seq(seq_t v)
{
    unsigned int c = 0;
    while (v)
    {
        v &= (v - 1);   /* remove lowest set bit */
        ++c;
    }
    return c;
}

/* cache of computed sequence values, 1‑based: a_cache[0] = a(1) */
static seq_t *a_cache   = NULL;   /* pointer to array */
static size_t cache_len = 0;      /* number of terms stored */
static size_t cache_cap = 0;      /* allocated capacity */

/* ensure the cache can hold at least 'needed' elements */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();          /* out of memory */
    a_cache = tmp;
    cache_cap = new_cap;
}

/* compute a(n) (n ≥ 1) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;        /* undefined for n = 0, but harmless */

    size_t target = (size_t)n;

    /* compute up to the requested index if needed */
    if (cache_len < target)
    {
        ensure_cache_capacity(target);

        /* initialise the very first term */
        if (cache_len == 0)
        {
            a_cache[0] = (seq_t)5;
            cache_len = 1;
        }

        /* generate subsequent terms */
        for (size_t i = cache_len; i < target; ++i)
        {
            seq_t prev = a_cache[i - 1];
            seq_t cand = prev + (seq_t)1;
            while (popcount_seq(cand ^ prev) != 5)
                ++cand;
            a_cache[i] = cand;
        }

        cache_len = target;
    }

    return a_cache[n - 1];
}
