/*
 * A235226  Numbers whose sum of digits is 15.
 * https://oeis.org/A235226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A235226: numbers whose digit sum is 15 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper: compute sum of decimal digits of an unsigned 64‑bit integer */
static inline unsigned int digit_sum_uint64(uint64_t x)
{
    unsigned int sum = 0;
    while (x > 0) {
        sum += (unsigned)(x % 10);
        x /= 10;
    }
    return sum;
}

/* The main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                 /* offset is 1, below offset is irrelevant */

    /* Static state for memoisation */
    static seq_t *cache    = NULL;
    static size_t  cap     = 0;
    static size_t  sz      = 0;  /* number of cached terms */
    static uint64_t cur     = 0;  /* last integer examined */

    /* Ensure cache can hold the requested term (0‑based index) */
    size_t target = (size_t)(n - 1);
    if (sz <= target) {
        /* Grow cache lazily */
        if (sz == 0) {
            cap = 128;
            cache = (seq_t*)malloc(sizeof(seq_t) * cap);
        } else {
            while (cap <= target) {
                size_t new_cap = cap << 1;
                if (new_cap < cap) new_cap = cap + 64;   /* avoid overflow */
                cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
                cap = new_cap;
            }
        }

        /* Generate terms until we have the desired count */
        while (sz <= target) {
            cur++;                                 /* next integer to test */
            if (digit_sum_uint64(cur) == 15) {
                cache[sz++] = (seq_t)cur;          /* store as seq_t */
            }
        }
    }

    return cache[target];
}
