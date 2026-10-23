/*
 * A043400  Numbers whose base-7 representation contains exactly four 1's.
 * https://oeis.org/A043400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers whose base‑7 representation contains exactly four 1's. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* External linkage */
seq_t a(idx_t n)
{
    /* Wrong if called with n == 0, but offset is 1; return 0 just in case. */
    if (n == 0)
        return 0;

    /* Static cache of already found terms. */
    static seq_t *cache = NULL;   /* dynamic array of terms */
    static size_t cache_cap = 0;  /* allocated capacity */
    static size_t cache_len = 0;  /* number of terms stored */

    /* Last examined integer (as seq_t). */
    static seq_t last = 0;

    /* Ensure cache space. */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* If we already have enough terms, return from cache. */
    if (cache_len >= (size_t)n)
        return cache[n - 1];

    /* Generate terms until we reach the requested index. */
    while (cache_len < (size_t)n) {
        /* Move to next integer. */
        last++;

        /* Count 1's in base‑7 representation. */
        seq_t x = last;
        unsigned count = 0;
        while (x > 0) {
            unsigned d = (unsigned)(x % 7);
            if (d == 1)
                count++;
            x /= 7;
        }

        /* Store if exactly four 1's. */
        if (count == 4) {
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = last;
        }
    }

    return cache[n - 1];
}
