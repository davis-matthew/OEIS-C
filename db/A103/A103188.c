/*
 * A103188  Numbers k such that k711 is prime.
 * https://oeis.org/A103188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_uint64, seq_malloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A103188: a(n) is the nth positive integer k such that
 * concatenating the decimal representation of k with "711"
 * (i.e. 1000·k + 711) yields a prime number. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime_uint64.h"
#include "../utils/seq_malloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t count = 0;
    static idx_t capacity = 0;
    static idx_t last_k = 0;       /* last k examined */

    if (!cache) {                 /* initial allocation */
        capacity = 16;
        cache = seq_malloc(capacity);
    }

    while (count < n) {
        ++last_k;
        /* build the concatenated number k*1000 + 711 without overflow */
        unsigned __int128 temp = (unsigned __int128)last_k * 1000ULL + 711ULL;
        uint64_t candidate = (uint64_t)temp;   /* fits for the needed n */

        if (is_prime_uint64(candidate)) {
            if (count == capacity) {          /* grow the cache */
                capacity <<= 1;
                cache = seq_realloc(cache, capacity);
            }
            cache[count++] = (seq_t)last_k;   /* store the found k */
        }
    }
    return cache[n - 1];
}
