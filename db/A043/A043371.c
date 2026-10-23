/*
 * A043371  Numbers having three 0's in base 6.
 * https://oeis.org/A043371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: a(n) is the n-th positive integer having exactly three
   zero digits in its base‑6 representation. */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache     = NULL;   /* array of found terms */
    static idx_t  cache_sz  = 0;      /* number of terms stored */
    static idx_t  capacity  = 0;      /* allocated length */
    static seq_t  next_test = 0;      /* next integer to examine */
    static idx_t  found     = 0;      /* number of terms already found */

    if (n == 0) return 0;             /* offset is 1; this case is unused */

    /* Return immediately if the term is already cached */
    if (n <= cache_sz)
        return cache[n - 1];

    /* Grow the cache to at least (n) elements */
    while (found < n) {
        /* Examine the next integer */
        next_test++;                  /* start from 1 */
        seq_t x = next_test;

        /* Count zeros in the base‑6 representation */
        seq_t t = x;
        unsigned int zeros = 0;
        while (t > 0) {
            if ((t % 6) == 0)
                zeros++;
            t /= 6;
        }

        if (zeros == 3) {            /* valid term */
            /* Grow the cache if needed */
            if (found + 1 > capacity) {
                idx_t new_cap = capacity ? capacity * 2 : 4;
                while (new_cap < found + 1) new_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();   /* out of memory */
                cache = tmp;
                capacity = new_cap;
            }
            cache[found] = x;
            found++;
        }
    }

    return cache[n - 1];
}
