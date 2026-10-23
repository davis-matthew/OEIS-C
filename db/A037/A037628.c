/*
 * A037628  Base 7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,3,0.
 * https://oeis.org/A037628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 7*a(n-1) + d_{n-1}, digits d repeat 2,3,0. Initial term:
 *            a(1) = 2. (Source: OEIS A037628)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for realloc, exit */

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* n is 1‑based; for n <= 0 the function returns 0 (offset guard) */
    if (n == 0) return 0;

    /* extend cache to hold terms up to n */
    if (n > cache_size)
    {
        idx_t new_size = n;
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            new_size * sizeof(seq_t));
        if (!new_cache) { /* out of memory – abort program */
            exit(1);
        }
        cache = new_cache;

        /* compute terms from old cache_size up to n-1 (0‑based index) */
        for (idx_t i = cache_size; i < new_size; ++i)
        {
            idx_t digit_index = i;          /* 0‑based index for the digit */
            seq_t digit;
            switch (digit_index % 3)
            {
                case 0: digit = 2; break;
                case 1: digit = 3; break;
                default: digit = 0; break;  /* digit_index % 3 == 2 */
            }

            seq_t prev = (i == 0) ? 0 : cache[i - 1];
            cache[i] = 7 * prev + digit; /* linear recurrence */
        }
        cache_size = new_size;
    }

    return cache[n - 1];  /* a(1) is at cache[0] */
}
