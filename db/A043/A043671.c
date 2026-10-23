/*
 * A043671  Numbers whose base-15 representation has exactly 4 runs.
 * https://oeis.org/A043671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑15 representation has exactly four runs
   a(n) is the n‑th integer whose base‑15 digits form exactly
   four maximal runs of equal digits. Computed by searching for
   integers in increasing order. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Return true if x has exactly four runs in base 15 */
static bool has_four_runs(seq_t x)
{
    unsigned char digits[128];
    size_t len = 0;

    /* Special case: 0 has a single run only */
    if (x == 0)
        return false;

    /* Extract base‑15 digits, least significant first */
    while (x > 0) {
        digits[len++] = (unsigned char)(x % (seq_t)15U);
        x /= (seq_t)15U;
    }

    /* Count runs from most significant digit */
    int runs = 1;
    int prev = digits[len - 1];
    for (size_t i = len - 1; i > 0; --i) {
        int d = digits[i - 1];
        if (d != prev) {
            ++runs;
            prev = d;
            if (runs > 4)   /* early exit */
                return false;
        }
    }
    return runs == 4;
}

seq_t a(idx_t n)
{
    /* Memoisation cache */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t current = 0;

    /* Grow cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        /* Find next integer with exactly 4 runs */
        do {
            ++current;
        } while (!has_four_runs(current));

        /* Append to cache */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* Allocation failure – terminate the program */
                exit(1);
            }
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = current;
    }

    return cache[n - 1];
}
