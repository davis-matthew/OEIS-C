/*
 * A021468  Decimal expansion of 1/464.
 * https://oeis.org/A021468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit n of the decimal expansion of 1/464 (recurrence:
 *            a(n) = a(n-1) - a(n-14) + a(n-15) for n>18)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache to store already computed digits */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* allocated capacity */
    static idx_t last = 0;          /* last computed index in cache */
    static uint64_t rem = 1;        /* remainder for long division */
    static int initialized = 0;

    /* initialise cache with the first term a(0) */
    if (!initialized) {
        cap = 4;                    /* initial capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* fallback on OOM */
        /* compute a(0) */
        seq_t digit = (seq_t)((rem * 10) / 464);
        rem = (rem * 10) % 464;
        cache[0] = digit;
        last = 0;
        initialized = 1;
    }

    /* grow the array if required */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap * 2 : 4;
        while (new_cap <= (size_t)n + 1) new_cap *= 2;
        seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_arr) return (seq_t)0; /* fallback on OOM */
        cache = new_arr;
        cap = new_cap;
    }

    /* if already computed, just return */
    if (n <= last) return cache[n];

    /* compute further terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t digit = (seq_t)((rem * 10) / 464);
        rem = (rem * 10) % 464;
        cache[i] = digit;
    }
    last = n;
    return cache[n];
}
