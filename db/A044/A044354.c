/*
 * A044354  Numbers n such that string 2,2 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th positive integer such that the decimal
 *            representation of a(n) contains the substring "22" but the
 *            decimal representation of a(n)-1 does not
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has_22
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation using a simple cache that generates numbers on demand.
   Prime avoidance or advanced sieving is unnecessary; the search is linear
   and efficient for n up to at least 10 000. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_22.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                      /* undefined for indices < 1 */

    static seq_t *cache       = NULL;  /* cached terms */
    static size_t cache_size = 0;      /* number of terms cached */
    static size_t cache_cap  = 0;      /* allocated capacity */
    static seq_t next_candidate = 22;  /* next integer to test */

    /* Generate terms until we have at least n terms stored. */
    while (cache_size < n)
    {
        seq_t x = next_candidate;

        if (has_22(x) && !has_22(x - 1))
        {
            if (cache_size == cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)        /* allocation failed */
                    return 0;
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = x;
        }

        ++next_candidate;
    }

    return cache[n - 1];
}
