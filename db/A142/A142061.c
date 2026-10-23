/*
 * A142061  Primes congruent to 20 mod 33.
 * https://oeis.org/A142061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 335, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: nth prime congruent to 20 modulo 33 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Helper: simple deterministic primality test for the small values needed */
static bool is_prime(seq_t n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if ((n & 1) == 0) return false;
    for (seq_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

/* Main sequence function -------------------------------------------------- */
seq_t a(idx_t n) {
    /* Static cache of found terms, growing as needed */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0, cache_cnt = 0;
    static idx_t next_k = 0;                /* k such that candidate = 20 + 33*k */

    if (n == 0) return 0;                  /* behaviour below offset is irrelevant */

    /* Generate terms until we have at least 'n' of them */
    while (cache_cnt < n) {
        seq_t candidate = 20 + 33 * next_k;
        ++next_k;

        if (is_prime(candidate)) {
            if (cache_cnt == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();            /* fatal if out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_cnt++] = candidate;
        }
    }

    return cache[n - 1];
}
