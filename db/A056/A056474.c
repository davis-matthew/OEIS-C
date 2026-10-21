/*
 * A056474  Number of palindromic structures using exactly five different
 *          symbols.
 * https://oeis.org/A056474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = stirling2( floor((n+1)/2), 5 )
 * Range:     u8 n <= 14, u16 n <= 20, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the sequence A056474.  The result is the Stirling number
 * of the second kind S( ⌊(n+1)/2⌋ , 5 ).
 * We cache the values as they are needed, updating the cache incrementally.
 * This uses only additions and small integer multiplications, and
 * works for any seq_t type that supports the standard arithmetic
 * operations. */
seq_t a(idx_t n)
{
    static idx_t cur_m = 0;          /* largest m whose value was cached   */
    static seq_t a_vals[6] = {1,0,0,0,0,0};  /* S(current_m, k) for k=0..5      */
    static seq_t *cache = NULL;     /* cache of S(m,5) for m=0..cur_m     */
    static idx_t cache_len = 0;     /* allocated size of cache            */

    /* m = floor((n+1)/2) */
    idx_t m = (n + 1) / 2;

    /* Initialise cache lazily */
    if (cache == NULL)
    {
        /* allocate a minimum size of 16 entries; the first entry (m=0) is 0 */
        cache_len = 16;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed; return 0 as a safe fallback */
        cache[0] = a_vals[5];   /* S(0,5) == 0 */
    }

    /* Extend the cache up to the required m */
    while (cur_m < m)
    {
        seq_t next[6];
        next[0] = 0;                      /* S(n,0) = 0 for n > 0 */
        for (int k = 1; k <= 5; ++k)
        {
            /* S(n,k) = k * S(n-1,k) + S(n-1,k-1) */
            next[k] = (seq_t)k * a_vals[k] + a_vals[k - 1];
        }

        /* Copy next into the current values */
        for (int k = 0; k <= 5; ++k)
            a_vals[k] = next[k];

        ++cur_m;

        /* Resize cache if necessary */
        if (cur_m >= cache_len)
        {
            size_t new_len = cache_len * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
            if (!tmp) return 0; /* allocation failed; safe fallback */
            cache = tmp;
            cache_len = new_len;
        }
        cache[cur_m] = a_vals[5];
    }

    return cache[m];
}
