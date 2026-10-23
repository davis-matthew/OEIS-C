/*
 * A154773  Numbers n such that 252n^2 is the average of a twin prime pair.
 * https://oeis.org/A154773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sqrt(A154673(n)/252)
 * Range:     u8 n <= 29, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A154773: numbers n such that 252 n² is the average of a twin prime pair. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;      /* stored values a(1), a(2), … */
    static size_t cache_len = 0;     /* number of stored terms */
    static size_t cache_cap = 0;     /* capacity of the cache */
    static idx_t last_tested = 0;    /* largest n already tested */

    /* Grow cache until we have at least n terms */
    while (cache_len < n) {
        ++last_tested;
        uint64_t m = (uint64_t)last_tested;
        __uint128_t prod = (__uint128_t)252 * m * m;  /* 252 n²  (128‑bit to avoid overflow) */
        uint64_t p_minus = (uint64_t)(prod - 1);
        uint64_t p_plus  = (uint64_t)(prod + 1);

        if (is_prime(p_minus) && is_prime(p_plus)) {
            /* Ensure sufficient space */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 32;
                seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_buf) exit(1);         /* fatal error: allocation failed */
                cache = new_buf;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)m;
        }
    }

    return cache[n - 1];
}
