/*
 * A174876  Numbers k such that the sum of squares of their digits > k.
 * https://oeis.org/A174876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   numbers k such that sum of squares of digits of k > k
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: full, pass, 50 terms through n = 50 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* Helper: sum of squares of decimal digits of a 64‑bit integer. */
static uint64_t digit_square_sum(uint64_t x)
{
    uint64_t sum = 0;
    while (x > 0) {
        uint64_t d = x % 10;
        sum += d * d;
        x /= 10;
    }
    return sum;
}

seq_t a(idx_t n) {
    /* static cache of all valid numbers (<= 99) */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* build cache once */
    if (cache == NULL) {
        for (idx_t k = 1; k <= 99; ++k) {
            /* digit_square_sum returns uint64_t */
            if (digit_square_sum(k) > k) {
                append_candidate(&cache, &cache_len, &cache_cap, (seq_t)k);
            }
        }
    }

    /* index starts at 1; return 0 for out-of-range */
    if (n == 0 || n > cache_len) {
        return (seq_t)0;
    }
    return cache[n - 1];
}
