/*
 * A039479  Numbers whose base-11 representation has the same nonzero number
 *          of 5's and 7's.
 * https://oeis.org/A039479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Procedure: iterate integers; keep those whose base‑11 representation has an equal, non‑zero count of digits 5 and 7. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static unsigned long long last_checked = 0;

static inline bool ok(unsigned long long x) {
    int c5 = 0, c7 = 0;
    if (x == 0) return false;
    while (x > 0) {
        unsigned int d = x % 11U;
        if (d == 5U) ++c5;
        else if (d == 7U) ++c7;
        x /= 11U;
    }
    return c5 > 0 && c5 == c7;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined, but harmless */

    if (n <= cache_len)
        return cache[n - 1];

    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    while (cache_len < (size_t)n) {
        /* find next valid number */
        ++last_checked;
        while (!ok(last_checked))
            ++last_checked;

        /* store it */
        if (cache_len == cache_cap) {
            cache_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
        }
        cache[cache_len++] = (seq_t)last_checked;
    }

    return cache[n - 1];
}
