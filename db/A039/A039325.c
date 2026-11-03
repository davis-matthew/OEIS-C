/*
 * A039325  Numbers whose base-7 representation has the same nonzero number of
 *          3's and 6's.
 * https://oeis.org/A039325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039325: computed by brute‑force search for numbers whose
 * base‑7 representation contains an equal non‑zero number of 3's and 6's.
 * The first term corresponds to a(1) = 27. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper to test whether a given unsigned integer has the required
 * property in its base‑7 representation. */
static inline bool has_equal_3_and_6_nonzero(uint64_t x)
{
    unsigned int c3 = 0, c6 = 0;
    while (x > 0) {
        uint32_t d = x % 7;
        if (d == 3)
            c3++;
        else if (d == 6)
            c6++;
        x /= 7;
    }
    return c3 == c6 && c3 > 0;
}

/* Return the n-th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    /* Static cache of previously computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static uint64_t next_num = 1;          /* next number to test */

    /* Expand the cache until it contains at least n terms. */
    while (cache_len < n) {
        if (has_equal_3_and_6_nonzero(next_num)) {
            /* Grow the cache if needed. */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp)
                    abort();                 /* Memory exhausted. */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_num;
        }
        next_num++;
    }

    return cache[n - 1];
}
