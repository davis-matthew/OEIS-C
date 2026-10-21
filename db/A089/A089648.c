/*
 * A089648  Numbers whose numbers of zeros and ones in binary representation
 *          differ at most by 1.
 * https://oeis.org/A089648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th non‑negative integer whose binary representation has
 *            the number of zeros and ones differing by at most 1
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps a static cache of the found terms.  Each
   call fills the cache until it contains at least n entries, then
   returns the requested term.  No external libraries are required; only
   stdlib.h for malloc/realloc. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache */
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;
    static idx_t cache_cap   = 0;
    static idx_t next_val    = 0;   /* next idx_t to test */

    /* Ensure we have at least n terms */
    while (cache_size < n)
    {
        /* Count ones in next_val */
        unsigned ones = 0;
        idx_t tmp = next_val;
        while (tmp) { ones++; tmp &= tmp - 1; }

        /* Determine bit length */
        unsigned bits;
        if (next_val == 0)
            bits = 1;              /* binary "0" has length 1 */
        else
        {
            bits = 0;
            idx_t t = next_val;
            while (t) { bits++; t >>= 1; }
        }

        unsigned zeros = bits - ones;
        if ( (ones > zeros ? ones - zeros : zeros - ones) <= 1 )
        {
            /* Expand cache if needed */
            if (cache_size == cache_cap)
            {
                idx_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0; /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)next_val;
        }
        next_val++;
    }

    return cache[n - 1];
}
