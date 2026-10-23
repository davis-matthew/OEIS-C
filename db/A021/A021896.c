/*
 * A021896  Decimal expansion of 1/892.
 * https://oeis.org/A021896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = digit (n+1) of the decimal expansion of 1/892 (offset 0:
 *            a(0) is the first digit after the decimal point)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */
#include <stdlib.h>   /* malloc, realloc, free */

/* Helper: ensure the cache has space for at least 'needed' digits. */
static void ensure_capacity(size_t needed, seq_t **cache, size_t *cap)
{
    if (*cap >= needed) return;
    size_t new_cap = (*cap > 0 ? *cap * 2 : needed);
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
    if (!tmp) { /* allocation failed – clean up and abort */
        free(*cache);
        *cache = NULL;
        *cap = 0;
        return;
    }
    *cache = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cached digits */
    static size_t cap    = 0;        /* current capacity of cache */
    static size_t next   = 0;        /* next digit index to compute */
    static uint64_t rem  = 1;        /* current remainder (initially 1) */

    /* Compute digits up to index n if needed */
    if (next <= n) {
        ensure_capacity(n + 1, &cache, &cap);
        const uint64_t denom = 892ULL;
        while (next <= n) {
            rem *= 10ULL;
            uint64_t digit = rem / denom;   /* 0 <= digit <= 9 */
            rem %= denom;
            cache[next] = (seq_t)digit;
            ++next;
        }
    }
    return cache[n];
}
