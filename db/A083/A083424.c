/*
 * A083424  a(n) = (5*4^n + (-2)^n)/6.
 * https://oeis.org/A083424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (5*4^n + (-2)^n)/6
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=3, a(n)=2*a(n-1)+8*a(n-2) */
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

static void ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;

    /* initial allocation */
    if (cache_cap == 0) {
        cache_cap = 4;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }

    while (cache_len <= n) {
        /* grow if needed */
        if (cache_len >= cache_cap) {
            cache_cap <<= 1;
            cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        }

        if (cache_len == 0) {          /* a(0) */
            cache[0] = (seq_t)1;
            ++cache_len;
            continue;
        }
        if (cache_len == 1) {          /* a(1) */
            cache[1] = (seq_t)3;
            ++cache_len;
            continue;
        }

        /* recurrence: a(n) = 2*a(n-1) + 8*a(n-2) */
        seq_t val = (cache[cache_len-1] * ((seq_t)2))
                    + (cache[cache_len-2] * ((seq_t)8));
        cache[cache_len] = val;
        ++cache_len;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
