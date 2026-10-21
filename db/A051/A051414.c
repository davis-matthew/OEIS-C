/*
 * A051414  a(n+1) = a(n) + sum of digits of a(n)^3.
 * https://oeis.org/A051414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n+1) = a(n) + digit_sum( a(n)^3 ), a(1) = 145
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 38, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Compute sum of decimal digits of a value. */
static seq_t digit_sum(seq_t x) {
    seq_t sum = 0;
    while (x > 0) {
        sum += x % 10;
        x   = x / 10;
    }
    return sum;
}

/* Ensure the dynamic array has space for `need` elements. */
static void ensure_capacity(size_t need) {
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) {
        new_cap *= 2;
    }
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) {
        /* Undefined; return 0 for safety. */
        return 0;
    }

    /* Initialize cache with the first term if necessary. */
    if (cache_len == 0) {
        ensure_capacity(1);
        cache[0] = (seq_t)145;
        cache_len = 1;
    }

    /* Compute terms up to the requested index. */
    size_t needed = (size_t)n;
    if (needed > cache_len) {
        ensure_capacity(needed);
        for (size_t i = cache_len; i < needed; ++i) {
            seq_t prev = cache[i - 1];
            seq_t cube = prev * prev;
            cube = cube * prev;
            seq_t add  = digit_sum(cube);
            cache[i]   = prev + add;
        }
        cache_len = needed;
    }

    return cache[n - 1];
}
