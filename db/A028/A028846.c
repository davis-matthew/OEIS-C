/*
 * A028846  Numbers whose product of digits is a power of 2.
 * https://oeis.org/A028846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 1108, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A028846: Numbers whose product of digits is a power of 2.
   Recurrence:
   a(0) = 0;
   For n ≥ 1, write n = 4k − r with 0 ≤ r ≤ 3.  Then
       a(n) = 10 · a(k − 1) + 2^(3 − r).
   (Here k = ceil(n/4); r = 4·k − n.)  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;       /* cache[i] holds a(i) for 0 ≤ i < cache_len */
static idx_t cache_len = 0;       /* number of entries already computed */
static idx_t cache_cap = 0;       /* allocated capacity of cache */

/* power of two table: 2^0..2^3 */
static const seq_t pow2[4] = {(seq_t)1, (seq_t)2, (seq_t)4, (seq_t)8};

seq_t a(idx_t n)
{
    if (n == 0)        /* a(0) is defined as 0 */
        return (seq_t)0;

    /* (1) Ensure enough space in cache */
    if (cache == NULL) {
        cache_cap = n + 1;
        if (cache_cap < 1) cache_cap = 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;
        cache[0] = (seq_t)0;
        cache_len = 1;
    } else if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap == 0 ? 1 : cache_cap;
        while (new_cap <= n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* (2) Compute all needed terms up to n */
    if (n + 1 > cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            idx_t r = (4 - (i & 3)) & 3;            /* 0 ≤ r ≤ 3  */
            idx_t k = (i + r) >> 2;                /* k = ceil(i/4) */
            cache[i] = cache[k - 1] * (seq_t)10 + pow2[3 - r];
        }
        cache_len = n + 1;
    }

    return cache[n];
}
