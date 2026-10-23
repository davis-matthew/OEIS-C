/*
 * A041032  Numerators of continued fraction convergents to sqrt(21).
 * https://oeis.org/A041032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 27, u64 n <= 55, u128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041032: Numerators of continued fraction convergents to sqrt(21).
   Recurrence: a(n) = 110*a(n-6) - a(n-12)   (n ≥ 12)
   Initial values:
   a(0)=4,  a(1)=5,  a(2)=9,  a(3)=23,  a(4)=32,  a(5)=55,
   a(6)=472, a(7)=527, a(8)=999, a(9)=2525, a(10)=3524, a(11)=6049 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    if (cache_size == 0) {
        const size_t init_size = 12;
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        if (!cache) abort();

        cache[0] = (seq_t)4;   cache[1] = (seq_t)5;   cache[2] = (seq_t)9;
        cache[3] = (seq_t)23;  cache[4] = (seq_t)32;  cache[5] = (seq_t)55;
        cache[6] = (seq_t)472; cache[7] = (seq_t)527; cache[8] = (seq_t)999;
        cache[9] = (seq_t)2525;cache[10] = (seq_t)3524;cache[11] = (seq_t)6049;

        cache_size = init_size;
    }

    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    const size_t new_size = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;

    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        cache[i] = 110 * cache[i - 6] - cache[i - 12];
    }
    cache_size = new_size;
    return cache[(size_t)n];
}
