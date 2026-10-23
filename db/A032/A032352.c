/*
 * A032352  Numbers k such that there is no prime between 10*k and 10*k+9.
 * https://oeis.org/A032352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2100, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A032352: Numbers k such that there is no prime between 10*k and 10*k+9.
   Computed by a brute‑force search. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Prime test for seq_t numbers.  Works for all sizes that can be represented
   by seq_t and is fast enough for the range of candidates we need. */
static bool isPrime(seq_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (seq_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

/* Cached sequence values and state for incremental generation. */
static seq_t *cache = NULL;       /* dynamic array storing generated k's   */
static size_t cache_size = 0;     /* number of elements stored              */
static size_t cache_cap   = 0;    /* allocated capacity for cache array    */
static seq_t next_candidate = 0;  /* smallest k not yet examined           */

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for offset 1, but harmless   */

    size_t target = (size_t)n;
    while (cache_size < target) {
        /* Ensure sufficient capacity for the cache. */
        if (cache_size >= cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
            while (new_cap < target) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();         /* out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Search for the next valid k. */
        seq_t k = next_candidate;
        for (;;) {
            seq_t start = k * (seq_t)10;   /* 10*k */
            bool has_prime = false;
            for (unsigned d = 0; d < 10 && !has_prime; ++d) {
                seq_t num = start + d;
                if (num >= 2 && isPrime(num)) has_prime = true;
            }
            if (!has_prime) {
                cache[cache_size++] = k;
                break;
            }
            ++k;
        }
        next_candidate = k + 1;   /* continue from the next number */
    }
    return cache[n - 1];
}
