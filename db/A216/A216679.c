/*
 * A216679  Numbers which can be written neither as a^2+b^2, nor as a^2+2*b^2,
 *          nor as a^2+3*b^2, nor as a^2+7*b^2, with a >= 0 and b >= 0.
 * https://oeis.org/A216679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: generate all integers which cannot be written as a^2 + k*b^2
   for k = 1,2,3,7.  a(1) is the first such integer (14). */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Cached results */
    static seq_t *cache = NULL;
    static idx_t cnt = 0;   /* number of terms stored */
    static idx_t cap = 0;   /* capacity of the cache array */
    static uint64_t max_checked = 0; /* largest integer tested so far */

    /* Initialise the cache array on first call */
    if (cap == 0) {
        cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
    }

    /* Generate terms until we have at least n of them cached */
    while (cnt < n) {
        uint64_t cand = max_checked + 1;   /* candidate integer to test */
        const uint8_t ks[4] = {1, 2, 3, 7};
        bool representable = false;        /* true if cand can be represented */

        /* Check all allowed k values */
        for (int ki = 0; ki < 4 && !representable; ++ki) {
            uint64_t k = ks[ki];
            /* For each b, test whether cand = a^2 + k*b^2 */
            for (uint64_t b = 0; ; ++b) {
                unsigned __int128 prod = (unsigned __int128)k * b * b;
                if (prod > cand) break;
                uint64_t kb = (uint64_t)prod;
                uint64_t rem = cand - kb;                      /* a^2 must equal rem */
                uint64_t a_sqrt = (uint64_t)floor_sqrt((seq_t)rem);
                if (a_sqrt * a_sqrt == rem) {                  /* perfect square found */
                    representable = true;
                    break;
                }
            }
        }

        /* If cand cannot be represented, store it */
        if (!representable) {
            if (cnt == cap) { /* grow the cache if needed */
                cap <<= 1;
                cache = (seq_t *)realloc(cache, sizeof(seq_t) * cap);
            }
            cache[cnt++] = (seq_t)cand;
        }

        max_checked = cand;
    }

    return cache[n - 1];
}
