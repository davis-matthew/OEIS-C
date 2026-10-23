/*
 * A167705  Composite numbers having four composite nearest neighbors.
 * https://oeis.org/A167705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A167705: composite numbers having four composite nearest neighbors
   (i.e. m-2, m-1, m+1, m+2 all composite).  This implementation
   performs an incremental brute‑force search with memoised results
   cached in a static array.  The search stops when the required
   nth term has been found. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Return true if n is composite (i.e., n>1 and not prime).   */
static inline bool is_composite(idx_t n)
{
    return n > 1U && !is_prime(n);
}

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;   /* terms cached so far */
    static idx_t cache_len   = 0;      /* number of cached terms    */
    static idx_t cache_cap   = 0;      /* allocated capacity       */
    static idx_t last_checked = 0;     /* last integer inspected   */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n)
    {
        ++last_checked;
        /* Candidate must be composite. */
        if (!is_composite(last_checked))
            continue;

        /* All four nearest neighbours must also be composite. */
        if (is_composite(last_checked - 2) &&
            is_composite(last_checked - 1) &&
            is_composite(last_checked + 1) &&
            is_composite(last_checked + 2))
        {
            /* Grow cache if necessary. */
            if (cache_len == cache_cap)
            {
                idx_t new_cap = (cache_cap < 16) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();   /* allocation failed */
                cache      = tmp;
                cache_cap  = new_cap;
            }
            /* Store the found composite number. */
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];  /* indices are 1‑based */
}
