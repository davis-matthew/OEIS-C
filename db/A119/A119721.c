/*
 * A119721  Numbers n such that 4*n^4 + 7 is prime.
 * https://oeis.org/A119721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 40, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A119721: Numbers n such that 4*n^4 + 7 is prime. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1 – n should be >=1 */

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_checked = 0; /* last n that was examined */

    while (cache_len < n) {
        /* Test value of 4 * last_checked^4 + 7 */
        seq_t m_seq   = (seq_t)last_checked;
        seq_t sq      = m_seq * m_seq;          /* m^2          */
        seq_t pow4    = sq * sq;                /* m^4          */
        seq_t val     = pow4 * (seq_t)4 + (seq_t)7;

        if (isPrimeGeneric(val)) {
            /* Grow cache if necessary */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0; /* allocation failure – return a dummy value */
                cache      = tmp;
                cache_cap  = new_cap;
            }
            cache[cache_len++] = m_seq;
        }
        last_checked++; /* next candidate */
    }

    return cache[n - 1];
}
