/*
 * A164388  Number of binary strings of length n with no substrings equal to
 *          0000 or 0011.
 * https://oeis.org/A164388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)-a(n-5) for n >= 5 * initial values:
 *            a(0)=1, a(1)=2, a(2)=4, a(3)=8, a(4)=14 * The function caches
 *            computed terms for efficiency
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 37, u64 n <= 76, u128 n <= 153,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;          /* cached values of the sequence  */
static size_t cache_len = 0;         /* current number of cached entries  */
static size_t cache_cap = 0;         /* allocated capacity */

static void ensure_cache(idx_t n)
{
    /* number of entries needed (indices 0 .. n inclusive) */
    size_t target_len = (size_t)n + 1;
    if (target_len <= cache_len)
        return;                      /* already enough */

    /* grow capacity if necessary */
    if (target_len > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 8;
        while (new_cap < target_len)
            new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            abort();                  /* allocation failed */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute the missing entries */
    for (size_t i = cache_len; i < target_len; ++i) {
        if (i == 0)
            cache[0] = (seq_t)1;
        else if (i == 1)
            cache[1] = (seq_t)2;
        else if (i == 2)
            cache[2] = (seq_t)4;
        else if (i == 3)
            cache[3] = (seq_t)8;
        else if (i == 4)
            cache[4] = (seq_t)14;
        else
            cache[i] = cache[i-1] + cache[i-2] + cache[i-3] - cache[i-5];
    }

    cache_len = target_len;
}

seq_t a(idx_t n)
{
    /* n is 0‑based index */
    ensure_cache(n);
    return cache[(size_t)n];
}
