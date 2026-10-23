/*
 * A106943  Primes of the form 3x^2+xy+6y^2, with x and y any integer.
 * https://oeis.org/A106943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime of the form 3*x^2 + x*y + 6*y^2 for integer x,y
 * Range:     u8 n <= 8, u16 n <= 938, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrime64, isqrt_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/isPrime64.h"        /* isPrime64(uint64_t n) */
#include "../utils/isqrt_uint64.h"    /* isqrt_uint64(uint64_t n) -> seq_t */

static bool is_representable(uint64_t p) {
    /* For each integer y such that 71*y^2 <= 12*p, check if 12*p-71*y^2 is a square
       and its sqrt ≡ y (mod 6) or sqrt ≡ -y (mod 6). */
    uint64_t max_y = (uint64_t)sqrt((double)(12ull * p) / 71.0);
    for (uint64_t y = 0; y <= max_y; ++y) {
        uint64_t y2 = y * y;
        if (71ull * y2 > 12ull * p) break;
        uint64_t D = 12ull * p - 71ull * y2;
        uint64_t s = (uint64_t)isqrt_uint64(D);
        if (s * s != D) continue;
        uint64_t ymod = y % 6;
        if (s % 6 == ymod || s % 6 == (6 - ymod) % 6) {
            return true;
        }
    }
    return false;
}

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;
static uint64_t last_trial = 2; /* next number to test for primality */

static void ensure_cache(idx_t needed) {
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < needed) new_cap *= 2;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) exit(1);
        cache = new_cache;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (cache_len < n) {
        ensure_cache(n);
        while (cache_len < n) {
            if (isPrime64(last_trial)) {
                if (is_representable(last_trial)) {
                    cache[cache_len++] = (seq_t)last_trial;
                }
            }
            ++last_trial;
        }
    }
    return cache[n-1];
}
