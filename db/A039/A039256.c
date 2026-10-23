/*
 * A039256  Numbers whose base-12 representation has the same number of 5's
 *          and 8's.
 * https://oeis.org/A039256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   base12_has_equal_5_8, ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base‑12 representation has the same number of
 * 5's and 8's (OEIS A039256).  The function computes the nth term
 * using a simple brute‑force scan, caching results for subsequent
 * calls.  The implementation is fully generic for seq_t and idx_t
 * and compiles as C11 and C++17. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ensure_capacity.h"
#include "../utils/base12_has_equal_5_8.h"

/* static cache */
static seq_t *cache = NULL;               /* array of found numbers */
static size_t cache_len = 0;               /* number of cached values (a(1)…a(cache_len)) */
static size_t cache_cap = 0;               /* capacity of the cache array */
static idx_t last_scanned = 0;             /* next integer to test after the last cached one */

seq_t a(idx_t n)
{
    /* Guard against n == 0, though the sequence is defined for n >= 1 */
    if (n == 0) return 0;

    /* Generate and cache values up to index n on demand */
    while (cache_len < (size_t)n) {
        /* Find the next integer that satisfies the base‑12 property */
        while (!base12_has_equal_5_8((seq_t)last_scanned))
            ++last_scanned;

        /* Ensure the cache array has space for another element */
        ensure_capacity(cache_len + 1, &cache_cap,
                        (void **)&cache, sizeof(seq_t));

        /* Store the found number and advance state */
        cache[cache_len] = (seq_t)last_scanned;
        ++cache_len;
        ++last_scanned;
    }

    return cache[(size_t)n - 1];
}
