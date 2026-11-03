/*
 * A031966  Numbers with exactly four distinct base-7 digits.
 * https://oeis.org/A031966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find numbers whose base‑7 representation uses exactly four distinct digits.
   a(1) = 360, a(2) = 361, …   The function generates terms sequentially and
   caches them for subsequent calls. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* helper: counts the distinct digits in base 7 of x */
static bool has_four_distinct_base7_digits(seq_t x)
{
    unsigned mask = 0;
    while (x) {
        unsigned digit = (unsigned)(x % 7);   /* digit < 7 */
        mask |= 1u << digit;
        x /= 7;
    }
    /* count bits set in mask */
    unsigned cnt = 0;
    while (mask) {
        cnt += mask & 1u;
        mask >>= 1;
    }
    return cnt == 4;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1, guard against illegal use */

    /* static state */
    static seq_t *cache = NULL;  /* array of found terms */
    static size_t cache_cap = 0; /* allocated capacity */
    static idx_t found = 0;      /* number of terms already found */
    static seq_t next = 1;       /* next candidate to test */

    /* return cached value if available */
    if (n <= found) {
        return cache[n - 1];
    }

    /* ensure capacity for new terms */
    if (cache == NULL) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
    }

    /* generate terms until we have at least n of them */
    while (found < n) {
        ++next;  /* next integer to examine */
        if (has_four_distinct_base7_digits(next)) {
            /* grow array if needed */
            if (found + 1 > cache_cap) {
                size_t new_cap = cache_cap * 2;
                if (new_cap < (size_t)(found + 1)) {
                    new_cap = (size_t)(found + 1);
                }
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[found++] = next;
        }
    }

    return cache[n - 1];
}
