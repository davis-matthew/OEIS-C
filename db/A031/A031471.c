/*
 * A031471  Numbers whose base-5 representation has one more 0 than 4's.
 * https://oeis.org/A031471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Condition: base-5 representation has one more 0 than 4's */
/* Implementation: search by iterating integers and counting base-5 digits */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for already found terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static seq_t last_checked = 0;

    if (n == 0)
        return 0; /* undefined for n < 1, return 0 as safe fallback */

    /* Return cached value if already computed */
    if (len >= (size_t)n)
        return cache[(size_t)n - 1];

    /* Extend cache until we have n terms */
    while (len < (size_t)n) {
        /* Move to next integer */
        last_checked++;
        seq_t val = last_checked;
        seq_t t = val;
        unsigned zeros = 0;
        unsigned fours = 0;

        /* Count digits in base 5 */
        while (t > 0) {
            int d = (int)(t % 5);
            if (d == 0)
                zeros++;
            else if (d == 4)
                fours++;
            t /= 5;
        }

        /* Condition check */
        if (zeros == fours + 1) {
            /* Ensure capacity */
            if (len == cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    exit(1); /* out of memory */
                cache = tmp;
                cap = new_cap;
            }
            cache[len++] = val;
        }
    }

    return cache[(size_t)n - 1];
}
