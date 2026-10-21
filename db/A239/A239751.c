/*
 * A239751  Numbers whose number of nonzero binary digits is less than their
 *          number of nonzero decimal digits.
 * https://oeis.org/A239751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = numbers n such that popcount(n) <
 *            number_of_nonzero_decimal_digits(n)
 * Range:     u8 n <= 9, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   count_nonzero_decimal_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/count_nonzero_decimal_digits.h"

/* popcount of a 64‑bit integer using only bitwise ops */
static unsigned popcount_uint64(uint64_t x)
{
    unsigned cnt = 0;
    while (x) {
        cnt += x & 1U;
        x >>= 1;
    }
    return cnt;
}

/* static cache of computed terms */
static seq_t  *cache      = NULL;
static size_t cache_cap   = 0;   /* capacity of cache */
static size_t term_count  = 0;   /* number of terms stored */
static uint64_t last_tested = 0; /* last candidate checked */

/* returns the nth term of A239751 (1‑based index) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* out of range, arbitrary */

    /* initial allocation if needed */
    if (cache == NULL) {
        cache_cap = (size_t)n > 10 ? (size_t)n : 10;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) abort();
    }

    /* enlarge cache if necessary */
    while (n > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < cache_cap) new_cap = cache_cap + 10;  /* overflow guard */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* generate terms until we have at least n */
    while (term_count < (size_t)n) {
        uint64_t candidate = ++last_tested;
        unsigned pc = popcount_uint64(candidate);
        unsigned nz = count_nonzero_decimal_digits(candidate);
        if (pc < nz) {
            cache[term_count++] = (seq_t)candidate;
        }
    }

    return cache[n - 1];
}
