/*
 * A037691  Base-7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2,0,3.
 * https://oeis.org/A037691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 7*a(n-1) + a(n-4) - 7*a(n-5) (n ≥ 6) Initial values:
 *            a(1)=1, a(2)=9, a(3)=63, a(4)=444, a(5)=3109 The function uses
 *            lazy memoisation with a dynamically growing cache. It works for
 *            both C and C++ builds, regardless of whether seq_t is a simple
 *            integral type or an arbitrary‑precision class
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t   cached_max = 0;   /* largest n whose value is stored */
static size_t  capacity   = 0;   /* number of slots allocated (index 0 unused) */

/* Allocate or grow the cache so that it can hold at least indices 0..n */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= capacity)
        return;

    size_t new_cap = capacity ? capacity * 2 : 16;
    while (new_cap < (size_t)n + 1)
        new_cap <<= 1;

#ifdef __cplusplus
    /* C++: allocate a new array, copy old elements, delete old array */
    seq_t *tmp = new seq_t[new_cap];
    for (size_t i = 0; i < capacity; ++i)
        tmp[i] = cache[i];
    delete[] cache;
    cache = tmp;
#else
    /* C: use realloc */
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    if (!tmp) { /* allocation failed – leave cache unchanged */
        return;
    }
    cache = tmp;
#endif
    capacity = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                /* undefined below offset */

    ensure_cache(n);
    if (!cache)                    /* allocation failure */
        return 0;

    /* Initialise the first five terms once */
    if (cached_max < 5) {
        cache[1] = 1;
        cache[2] = 9;
        cache[3] = 63;
        cache[4] = 444;
        cache[5] = 3109;
        cached_max = 5;
    }

    /* Compute any missing values up to n */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        cache[i] = cache[i - 1] * 7 + cache[i - 4] - cache[i - 5] * 7;
    }
    cached_max = n;
    return cache[n];
}
