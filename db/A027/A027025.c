/*
 * A027025  a(n) = T(n,n+3), T given by A027023.
 * https://oeis.org/A027025
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000213(n+3) -4*(n+1)
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 34, u64 n <= 71, u128 n <= 143,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived recurrence: a(n) = 3*a(n-1) - 2*a(n-2) - a(n-4) + a(n-5)  (n ≥ 8) */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static int base_set = 0;

static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (cache_cap < needed)
    {
        size_t new_cap = needed * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 3)
        return 0; /* values below the offset are undefined */

    /* make sure we have room for at least 8 entries for the base cases */
    ensure_cache(n > 7 ? n : 7);

    if (!base_set)
    {
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 1;
        cache[4] = 11;
        cache[5] = 33;
        cache[6] = 77;
        cache[7] = 161;
        base_set = 1;
    }

    for (idx_t i = 8; i <= n; ++i)
    {
        cache[i] = 3 * cache[i - 1] - 2 * cache[i - 2] - cache[i - 4] + cache[i - 5];
    }

    return cache[n];
}
