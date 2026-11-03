/*
 * A043502  Numbers having two 3's in base 10.
 * https://oeis.org/A043502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   This function generates the sequence of numbers that contain
 *            exactly two digits '3' in their decimal representation. It works
 *            by scanning integers in ascending order and collecting those
 *            that satisfy the condition. Cache keeps previously computed
 *            terms to support arbitrary order of calls
 * Range:     u8 n <= 3, u16 n <= 5720, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 59 terms through n = 9759 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: count how many times the digit 3 appears in x */
static bool has_exactly_two_3s(seq_t x) {
    unsigned count = 0;
    do {
        if ((x % 10) == 3) {
            ++count;
            if (count > 2) return false;          /* early exit */
        }
        x /= 10;
    } while (x > 0);
    return count == 2;
}

seq_t a(idx_t n) {
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_used = 0;
    static seq_t current = 0;
    static bool initialized = false;

    if (!initialized) {
        /* initial capacity 64, grow as needed */
        cache_cap = 64;
        cache = mem_new(seq_t, cache_cap);
        cache_used = 0;
        current = 0;
        initialized = true;
    }

    /* Generate terms until we have at least n of them */
    while (cache_used < n) {
        current++;                           /* next integer */
        if (has_exactly_two_3s(current)) {
            /* grow cache if full */
            if (cache_used == cache_cap) {
                cache_cap *= 2;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_used++] = current;
        }
    }

    return cache[n - 1];
}
