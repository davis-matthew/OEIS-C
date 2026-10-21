/*
 * A085897  Group the natural numbers such that the product of the n-th group
 *          is divisible by 2^n. (1), (2), (3,4), (5,6,7,8),
 *          (9,10,11,12,13,14), (15,16,17,18), ... Sequence contains the first
 *          term of the groups.
 * https://oeis.org/A085897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the first integer of the n-th group in the greedy *
 *            partition of natural numbers where the product of the n-th group
 *            * is divisible by 2^n (i.e., contains at least n twos in its
 *            prime * factorisation). The groups are formed sequentially: the
 *            first term * of each group is the smallest inte...
 * Range:     u8 n <= 22, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* Cached first terms a(0), a(1), ... */
static size_t cache_len = 0;          /* Number of cached terms */
static size_t cache_cap = 0;          /* Allocated capacity of cache */
static idx_t next_start = 1;          /* Next unused integer */

/* Ensure the cache can hold at least (len) entries */
static void ensure_cache_cap(size_t len)
{
    if (cache_len == cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < len) new_cap = len;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Count trailing zeros in the binary representation of x */
static inline idx_t v2(idx_t x)
{
    return (idx_t)__builtin_ctzll((unsigned long long)x);
}

seq_t a(idx_t n)
{
    /* Compute the sequence up to index n, using a greedy construction. */
    if (n >= cache_len)
    {
        idx_t g;
        /* Ensure storage for all new terms */
        if (n >= cache_cap) ensure_cache_cap((size_t)n + 1);
        for (g = cache_len; g <= n; ++g)
        {
            seq_t start = (seq_t)next_start;   /* a(g) */
            idx_t i = next_start;
            idx_t cum = 0;

            if (g == 0)
            {
                /* For n = 0 we need at least one number. */
                cum += v2(i);
                ++i;
            }
            else
            {
                while (cum < g)        /* need at least g twos */
                {
                    cum += v2(i);
                    ++i;
                }
            }

            /* Record the first term of this group */
            cache[g] = start;
            cache_len = (size_t)g + 1;
            /* Prepare the start for the next group */
            next_start = i;
        }
    }
    return cache[(size_t)n];
}
