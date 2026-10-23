/*
 * A043706  Numbers whose base-3 representation has an odd number of runs.
 * https://oeis.org/A043706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base-3 representation has an odd number of runs
   (OEIS A043706).  The function finds the smallest positive integers
   with an odd number of runs in ternary and returns the n‑th such integer. */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache of found numbers.  The cache is grown as needed. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0, cache_cap = 0;
    static idx_t next_candidate = 1;   /* next number to test */

    /* Return from cache if already computed. */
    if (n <= cache_size)
        return cache[n - 1];

    /* Continue searching from where we left off. */
    for (idx_t cand = next_candidate; ; ++cand)
    {
        /* Count runs of equal digits in base 3. */
        idx_t v = cand;
        int runs = 0;
        int last_digit = -1;
        while (v != 0)
        {
            int d = v % 3;
            if (d != last_digit)
            {
                ++runs;
                last_digit = d;
            }
            v /= 3;
        }

        /* Accept if the number of runs is odd. */
        if ((runs & 1) != 0)
        {
            /* Ensure capacity in the cache. */
            if (cache_size == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!p) abort();      /* unrecoverable memory allocation failure */
                cache = p;
                cache_cap = new_cap;
            }
            /* Store the value (cast to seq_t). */
            cache[cache_size++] = (seq_t)cand;

            /* If we have enough entries, finish. */
            if (cache_size >= n)
            {
                next_candidate = cand + 1;
                return cache[n - 1];
            }
        }
    }
}
