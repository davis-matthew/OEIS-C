/*
 * A039477  Numbers whose base-11 representation has the same nonzero number
 *          of 4's and 10's.
 * https://oeis.org/A039477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th positive integer whose base‑11
   representation contains the same non‑zero numbers of digits 4 and
   10.  The values are found by incremental search and stored in a
   static cache so that repeated calls in arbitrary order are fast. */
/*  Expected to be used with a seq_t that is either a fixed‑width
   unsigned integer or an arbitrary‑precision integer class. */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Offset is 1; behaviour for n < 1 is undefined */
    if (n == 0u) return 0;

    /* Static cache – sufficient for OEIS test data up to ~10 000 */
    #define CACHE_MAX 20000
    static seq_t cache[CACHE_MAX];
    static size_t cached = 0;
    static seq_t next_candidate = 1;

    /* Fill the cache until we have at least n terms */
    while (cached < (size_t)n)
    {
        /* Find the next value with equal non‑zero 4’s and 10’s */
        for (;;)
        {
            seq_t x = next_candidate;
            unsigned int c4 = 0, c10 = 0;

            /* Count digits in base 11 */
            while (x > 0)
            {
                seq_t d = x % (seq_t)11;
                if (d == 4u) ++c4;
                else if (d == 10u) ++c10;
                x /= (seq_t)11;
            }

            if (c4 == c10 && c4 > 0) break;  /* valid candidate found */

            ++next_candidate;
        }

        /* Store the found term */
        cache[cached++] = next_candidate;

        /* Advance to the next integer for the following search */
        ++next_candidate;
    }

    return cache[n - 1];
}
