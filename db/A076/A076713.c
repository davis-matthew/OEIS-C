/*
 * A076713  Harshad (Niven) triangular numbers: triangular numbers which are
 *          divisible by the sum of their digits.
 * https://oeis.org/A076713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 117, u32 n <= 14417, u64 n <= 15437,
 *            u128 n <= 15437, big any n
 * Validated: standard, pass, 64 terms through n = 9781 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: generate triangular numbers divisible by their digit sum (Harshad) */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* return the sum of decimal digits of x */
static seq_t digit_sum_seq(seq_t x) {
    seq_t sum = 0;
    while (x != 0) {
        sum += x % 10;
        x /= 10;
    }
    return sum;
}

/* cache of found terms, last processed triangular index */
static seq_t *cache          = NULL;
static size_t cache_len      = 0;
static size_t cache_cap      = 0;
static idx_t  last_k         = 0;

/* external linkage as required */
seq_t a(idx_t n) {
    /* nothing is defined for indices <= 0 */
    if (n == 0)
        return (seq_t)0;

    /* extend the cache until we have at least n terms */
    while (cache_len < n) {
        ++last_k;                            /* next triangular index */
        seq_t tri = (seq_t)last_k * ((seq_t)last_k + 1) / 2;

        seq_t dsum = digit_sum_seq(tri);
        if (dsum != 0 && (tri % dsum) == 0) {  /* Harshad criterion */
            /* grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 64;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = tri;
        }
    }
    return cache[n - 1];
}
