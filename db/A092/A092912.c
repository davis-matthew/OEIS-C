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
 * Validated: standard, pass, 70 terms through n = 7576 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Return a bitmask of decimal digits present in x (0-9). */
static uint16_t digit_mask_u64(uint64_t x)
{
    uint16_t m = 0;
    if (x == 0) return 1 << 0;
    while (x != 0) {
        m |= 1 << (x % 10);
        x /= 10;
    }
    return m;
}

/* Test whether k satisfies the definition. */
static bool check_k(uint64_t k)
{
    /* k must contain the digit 1 (necessary for the divisor 1). */
    uint16_t maskK = digit_mask_u64(k);
    if ((maskK & (1 << 1)) == 0) return false;

    /* Scan divisors up to sqrt(k). */
    for (uint64_t d = 1; d <= k / d; ++d) {
        if (k % d == 0) {
            uint16_t maskD = digit_mask_u64(d);
            if (maskD & ~maskK) return false;

            uint64_t d2 = k / d;
            if (d2 != d) {
                uint16_t maskD2 = digit_mask_u64(d2);
                if (maskD2 & ~maskK) return false;
            }
        }
    }
    return true;
}

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
        cache = mem_new(seq_t, cache_cap);
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
                cache = mem_resize(seq_t, cache, cache_cap);
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
