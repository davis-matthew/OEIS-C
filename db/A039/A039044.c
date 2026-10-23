/*
 * A039044  Numbers whose base-7 representation has the same number of 3's and
 *          4's.
 * https://oeis.org/A039044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 109, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A039044: Numbers whose base‑7 representation has the same number
 * of 3's and 4's.  a(1)=0, a(2)=1, …  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Return the n‑th (1‑based) term of the sequence.  */
seq_t a(idx_t n)
{
    /* Cache of known terms.  Cached terms are stored in ascending order,
     * so a(n) can be returned directly if it has already been computed. */
    static seq_t *cache = NULL;      /* dynamic array of terms          */
    static size_t cache_cap = 0;      /* current capacity of cache       */
    static idx_t cache_size = 0;      /* number of terms cached          */
    static seq_t next_val = 0;       /* next integer to examine         */

    if (n == 0) return 0;            /* not defined for offset 1, but harmless */

    /* If the requested term is already cached, return it. */
    if (n <= cache_size)
        return cache[n - 1];

    /* Extend the cache until we have at least n terms. */
    while (cache_size < n)
    {
        /* Count 3’s and 4’s in the base‑7 representation of next_val. */
        unsigned cnt3 = 0, cnt4 = 0;
        seq_t tmp = next_val;
        if (tmp != 0)
        {
            while (tmp > 0)
            {
                int d = (int)(tmp % 7);
                if (d == 3) ++cnt3;
                else if (d == 4) ++cnt4;
                tmp /= 7;
            }
        }

        /* If the counts are equal, store the term. */
        if (cnt3 == cnt4)
        {
            /* Grow the cache if necessary. */
            if (cache_size >= cache_cap)
            {
                size_t newcap = cache_cap ? cache_cap * 2 : 8;
                seq_t *newptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!newptr)            /* allocation failure: abort */
                    return 0;
                cache = newptr;
                cache_cap = newcap;
            }
            cache[cache_size++] = next_val;
        }

        /* Move to the next integer. */
        ++next_val;
    }

    return cache[n - 1];
}
