/*
 * A043498  Numbers having two 2's in base 10.
 * https://oeis.org/A043498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 5720, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates numbers having exactly two 2's in base 10 (by brute‑force search) */
#include <stdlib.h>

static seq_t *cache = NULL;        /* cache of computed terms */
static idx_t cache_cap = 0;        /* allocated capacity of the cache */
static idx_t cache_len = 0;        /* number of terms actually stored */

/* Return the nth term of the sequence (1‑based index) */
seq_t a(idx_t n)
{
    /* Handle the offset: a(1) = 22 */
    if (n == 0) return 0;

    /* grow the cache if it does not yet contain n terms */
    if (cache_len < n)
    {
        /* allocate or grow the cache */
        if (cache_cap < n)
        {
            idx_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
            if (new_cap < n) new_cap = n;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            /* In strict C we cast the result of realloc. In C++ it is
               implicitly converted. */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Invariant for the search: current holds the next decimal number
           to examine, found indicates how many terms have been found so far. */
        static seq_t current = 22;   /* next integer to test */
        static idx_t found   = 0;    /* number of terms stored so far */

        /* Continue searching until we have the required number of terms */
        while (cache_len < n)
        {
            /* Count the number of digit '2' in current */
            seq_t tmp = current;
            unsigned cnt = 0;
            while (tmp > 0)
            {
                if ((tmp % 10) == 2u) ++cnt;
                tmp /= 10;
            }
            if (cnt == 2u)
            {
                cache[cache_len++] = current;
                ++found;
            }
            ++current;
        }
    }

    /* Return the requested term (1‑based index) */
    return cache[n - 1];
}
