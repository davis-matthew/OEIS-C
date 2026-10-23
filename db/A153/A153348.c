/*
 * A153348  Numbers k such that 16*k+3 is not prime.
 * https://oeis.org/A153348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 185, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   free_seq_array, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that 16*k+3 is not prime (search) */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <stddef.h>

#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"

/* Very small primality test for 64‑bit values */
static bool is_prime_uint64(uint64_t v)
{
    if (v < 2) return false;
    if (v % 2 == 0) return v == 2;
    if (v % 3 == 0) return v == 3;
    uint64_t r = (uint64_t)sqrt((double)v);
    for (uint64_t i = 5; i <= r; i += 6) {
        if (v % i == 0 || v % (i + 2) == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n < 1 */

    /* Static cache holding the terms we have found so far  */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;  /* allocated capacity */
    static size_t cache_len  = 0;  /* number of terms stored */
    static uint64_t next_k   = 1;  /* next integer k to test */

    /* If we need a larger cache, grow it */
    if (cache_len < n && cache_cap < n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < n) new_cap = n;

        seq_t *new_cache = seq_alloc_new_array(new_cap);
        if (!new_cache) exit(EXIT_FAILURE);

        /* copy existing values to the new array */
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];

        /* free old storage if any */
        if (cache) free_seq_array(cache);

        cache     = new_cache;
        cache_cap = new_cap;
    }

    /* Generate terms until we have the n‑th one */
    while (cache_len < n) {
        uint64_t val = 16ULL * next_k + 3ULL;
        if (!is_prime_uint64(val)) {
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
