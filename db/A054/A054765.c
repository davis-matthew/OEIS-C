/*
 * A054765  a(n+2) = (2n+3)*a(n+1) + (n+1)^2*a(n), a(0) = 0, a(1) = 1.
 * https://oeis.org/A054765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+2) = (2n+3)*a(n+1) + (n+1)^2*a(n), a(0)=0, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoises the sequence in a static array so that calls in any
   order run in amortised O(1) time per term.  It works for all underlying
   integer types (uint8_t … unsigned __int128 or arbitrary‑precision),
   relying only on the operators +, -, *, /, % and the conversion from an
   integer literal to seq_t. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Special cases that never require memoisation */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of valid entries in cache   */
    static size_t cache_cap = 0;   /* allocated capacity in cache       */

    size_t target = (size_t)n;

    /* Allocate or grow the cache to a suitable capacity */
    if (cache_len <= target)
    {
        size_t new_cap = cache_cap == 0 ? 2 : cache_cap;
        while (new_cap <= target) new_cap <<= 1;            /* double until enough */

        if (cache == NULL)
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        else
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));

        cache_cap = new_cap;
    }

    /* Compute terms up to 'target', appending to the cache */
    for (size_t i = cache_len; i <= target; ++i)
    {
        if (i == 0)                 cache[0] = (seq_t)0;
        else if (i == 1)             cache[1] = (seq_t)1;
        else
        {
            idx_t k = (idx_t)(i - 2);                /* k = i-2  */
            seq_t mult1 = ((seq_t)2 * ((seq_t)k) + (seq_t)3);  /* 2k+3   */
            seq_t term1  = cache[k + 1] * mult1;            /* (2k+3)*a(k+1) */
            seq_t mult2  = ((seq_t)(k + 1) * ((seq_t)(k + 1)));/* (k+1)^2 */
            seq_t term2  = cache[k] * mult2;                /* (k+1)^2*a(k)  */
            cache[i] = term1 + term2;
        }
    }

    cache_len = target + 1;  /* we now have entries for indices 0 … target */
    return cache[n];
}
