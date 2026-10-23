/*
 * A163446  a(n) = 16*a(n-1) - 62*a(n-2) for n > 1; a(0) = 1, a(1) = 10.
 * https://oeis.org/A163446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 10, a(n) = 16*a(n-1) - 62*a(n-2) n > 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

static void ensure_cached(idx_t n)
{
    if (n < cache_size)
        return;

    /* new capacity needed */
    size_t new_cap = (size_t)(n + 1);

    /* realloc the cache; cast to seq_t* for C/C++ compatibility */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        return;                       /* allocation failure – caller sees stale data */

    cache = tmp;

    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0)
            cache[0] = (seq_t)1;
        else if (i == 1)
            cache[1] = (seq_t)10;
        else
            cache[i] = cache[i - 1] * (seq_t)16 - cache[i - 2] * (seq_t)62;
    }
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cached(n);
    return cache[n];
}
