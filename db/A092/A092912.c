/*
 * A092912  Numbers k all of whose divisors contain only digits that occur at
 *          least once in k.
 * https://oeis.org/A092912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the n-th integer k such that every divisor of k consists
 *            of digits that appear in k. (OEIS A092912)
 * Range:     u8 n <= 37, u16 n <= 3756, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   check_k, digit_mask_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/check_k.h"
#include "../utils/digit_mask_u64.h"

/* Return the n-th term (n >= 1). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_cnt = 0;
    static uint64_t last_checked = 1;

    if (n == 0) return 0;          /* unused, just for safety */

    /* Ensure cache has slots for the first term. */
    if (cache_cnt == 0) {
        cache_cap = 256;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();       /* allocation failure */
        cache[0] = (seq_t)1;       /* a(1) = 1 */
        cache_cnt = 1;
    }

    /* If already computed, return immediately. */
    if (n <= cache_cnt) return cache[n - 1];

    /* Generate terms until we reach the n-th. */
    for (uint64_t k = last_checked + 1;; ++k) {
        if (check_k(k)) {
            /* Append to cache. */
            if (cache_cnt >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) abort();
            }
            cache[cache_cnt++] = (seq_t)k;
            if (cache_cnt == n) {
                last_checked = k;
                return cache[n - 1];
            }
        }
    }
}
