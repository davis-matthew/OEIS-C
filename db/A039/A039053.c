/*
 * A039053  Numbers whose base-8 representation has the same number of 0's and
 *          4's.
 * https://oeis.org/A039053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   ensure_capacity, has_equal_zero_four
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search of natural numbers,
   testing if the base‑8 representation has an equal number
   of digits 0 and 4.  The sequence is OEIS A039053. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/ensure_capacity.h"
#include "../utils/has_equal_zero_four.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t next_candidate = 1;

    if (n == 0) return 0; /* undefined, but return 0 for safety */

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < (size_t)n)
    {
        if (has_equal_zero_four(next_candidate))
        {
            /* Ensure enough capacity in the cache array. */
            if (cache_cap <= cache_len)
                ensure_capacity(cache_len + 1, &cache_cap,
                                (void **)&cache, sizeof(seq_t));
            cache[cache_len++] = next_candidate;
        }
        next_candidate++;
    }

    return cache[(size_t)n - 1];
}
