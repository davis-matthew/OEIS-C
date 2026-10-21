/*
 * A062382  Positive numbers whose product of digits is 5 times their sum.
 * https://oeis.org/A062382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 126, u32 n <= 500, u64 n <= 500, u128 n <= 500,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Positive numbers whose product of digits is 5 times their sum */

#include <stdint.h>
#include <stdlib.h>

/* Return the nth term of sequence A062382 (1‑based indexing). */
seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_sz   = 0;
    /* next natural number to test */
    static uint64_t next_num = 1;

    /* Allocate initial cache capacity if necessary */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    }

    /* Generate terms until we have at least n of them */
    while (cache_sz < n) {
        /* compute digit sum and product of next_num */
        seq_t sum  = 0;
        seq_t prod = 1;
        uint64_t x = next_num;
        while (x > 0) {
            uint8_t d = (uint8_t)(x % 10);
            sum  += d;
            prod *= d;
            x   /= 10;
        }

        if (prod == 5 * sum) {
            /* store in cache, growing if necessary */
            if (cache_sz == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
            }
            cache[cache_sz++] = (seq_t)next_num;
        }

        ++next_num;   /* move to the next candidate */
    }

    return cache[n - 1];
}
