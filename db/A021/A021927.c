/*
 * A021927  Decimal expansion of 1/923.
 * https://oeis.org/A021927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A021927: decimal expansion of 1/923.
   a(0) = 0, a(1) = 0, a(2) = 1, ...
   Digit a(n) is the n‑th digit (0‑based) of 1/923 in decimal.
   The value is computed by long division with a static cache
   so that repeated calls are fast and non‑monotonic. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* compute the nth digit of 1/923 – same definition as OEIS A021927 */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cached digits */
    static size_t cache_size = 0;     /* number of digits stored */
    static size_t cache_cap  = 0;     /* allocated capacity */
    static uint32_t rem = 1;          /* current remainder (0..922) */

    /* If already computed, return immediately */
    if ((idx_t)cache_size > n)
        return cache[(size_t)n];

    /* Ensure enough capacity */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < needed) new_cap *= 2;
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return 0; /* out of memory – fall back to 0 */
        if (cache) {
            memcpy(new_cache, cache, cache_size * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Generate digits from cache_size up to n (inclusive) */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        uint32_t t = rem * 10U;          /* shift remainder left by one decimal place */
        uint32_t digit = t / 923U;       /* next decimal digit */
        rem = t % 923U;                  /* new remainder */
        cache[i] = (seq_t)digit;         /* store digit */
    }
    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
