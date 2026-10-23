/*
 * A043787  Numbers n such that number of runs in base 3 representation of n
 *          is congruent to 2 mod 6.
 * https://oeis.org/A043787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n‑th positive integer whose number of runs
   in its base‑3 representation is congruent to 2 modulo 6. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* Last integer examined (not necessarily in cache) */
    static seq_t last_checked = 0;

    if (n == 0) return 0; /* offset is 1; this value is unused */

    /* Generate terms until we have at least n terms */
    while (cache_len < n) {
        seq_t val;

        /* Find the next integer that satisfies the condition */
        for (;;) {
            ++last_checked;
            val = last_checked;

            /* Count runs in base‑3 representation of val */
            seq_t temp  = val;
            seq_t prev  = temp % 3;
            temp /= 3;
            seq_t runs = 1;          /* first run */
            while (temp > 0) {
                seq_t digit = temp % 3;
                if (digit != prev) ++runs;
                prev = digit;
                temp /= 3;
            }

            if ((runs % 6) == 2)
                break;   /* val satisfies the property */
        }

        /* Grow cache if needed */
        if (cache_len >= cache_cap) {
            cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        cache[cache_len++] = val;
    }

    /* Return the requested term */
    return cache[n - 1];
}
