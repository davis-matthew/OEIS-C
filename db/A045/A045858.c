/*
 * A045858  Numbers whose square has initial digit '4'.
 * https://oeis.org/A045858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 36, u32 n <= 6093, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers whose square starts with digit 4. */
/* a(1) = 2, a(2) = 7, a(3) = 20, ... (OEIS A045858) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t length = 0, capacity = 0;
    static idx_t candidate = 1;   /* next integer to test */

    /* Extend cache to contain at least n terms */
    while (length < (size_t)n) {
        /* Test next candidate */
        ++candidate;
        seq_t sq = (seq_t)candidate * candidate;

        /* Find leading decimal digit of sq */
        seq_t d = sq;
        while (d >= 10)
            d = d / 10;

        if (d == 4) {
            /* Ensure capacity before insertion */
            if (length == capacity) {
                size_t newcap = capacity ? capacity * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp)
                    exit(1);          /* allocation failure */
                cache = tmp;
                capacity = newcap;
            }
            cache[length++] = (seq_t)candidate;
        }
    }

    return cache[n - 1];   /* n is 1‑based */
}
