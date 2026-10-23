/*
 * A008066  Coordination sequence T1 for Zeolite Code CHA.
 * https://oeis.org/A008066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 196, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1,a(1)=4,a(2)=9,a(3)=17,a(4)=29,a(5)=45,a(6)=64,a(7)=85,
   a(8)=110,a(9)=140;
   for n ≥ 10:
       a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - a(n-4) + a(n-5)
                - 2*a(n-6) + 2*a(n-7) - 2*a(n-8) + a(n-9). */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static size_t capacity = 0;

    if (cache == NULL) {
        capacity = 64;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        const seq_t base[10] = {1,4,9,17,29,45,64,85,110,140};
        for (idx_t i = 0; i < 10; ++i) cache[i] = base[i];
        cache_size = 10;
    }

    if (n < cache_size) return cache[n];

    while (cache_size <= n) {
        if (cache_size >= capacity) {
            capacity <<= 1;
            cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
            if (!cache) return (seq_t)0;
        }
        seq_t val = 0;
        val += 2 * cache[cache_size-1];
        val -= 2 * cache[cache_size-2];
        val += 2 * cache[cache_size-3];
        val -=      cache[cache_size-4];
        val +=      cache[cache_size-5];
        val -= 2 * cache[cache_size-6];
        val += 2 * cache[cache_size-7];
        val -= 2 * cache[cache_size-8];
        val +=      cache[cache_size-9];
        cache[cache_size] = val;
        cache_size++;
    }

    return cache[n];
}
