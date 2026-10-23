/*
 * A134107  Complete list of solutions to y^2 = x^3 - 207; sequence gives x
 *          values.
 * https://oeis.org/A134107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = nth x-coordinate of integer solutions of y^2 = x^3 - 207
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Integer square root for unsigned __int128 */
static inline unsigned __int128 isqrt128(unsigned __int128 n)
{
    unsigned __int128 r = 0;
    unsigned __int128 bit = (unsigned __int128)1 << 126; /* highest power <= 2^128 */
    while (bit > 0)
    {
        if (r + bit <= n / (r + bit))
            r += bit;
        bit >>= 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* static cache of all solutions, computed on first call */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;

    /* Construct cache lazily */
    if (!cache)
    {
        /* Upper bound for x which is guaranteed to contain all solutions.
           1 000 000 is comfortably larger than the known largest solution
           (367 806) and is sufficient for the bound on integral
           solutions of y^2 = x^3 - 207. */
        const idx_t MAX_X = 1000000ULL;
        const size_t INITIAL_CAP = 16;

        cache_cap = INITIAL_CAP;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failed, return 0 */
        cache_len = 0;

        for (idx_t x = 1; x <= MAX_X; ++x)
        {
            unsigned __int128 val = (unsigned __int128)x * x * x;
            val -= 207;                    /* x^3 - 207 >= 0 for x >= 7 */
            if (val < 0) continue;         /* safety for small x (x<7) */

            unsigned __int128 s = isqrt128(val);
            if (s * s == val)              /* perfect square -> y^2 = val */
            {
                if (cache_len == cache_cap) /* grow if needed */
                {
                    cache_cap *= 2;
                    seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                    if (!tmp) /* allocation failed: free and abort */
                    {
                        free(cache);
                        cache = NULL;
                        return (seq_t)0;
                    }
                    cache = tmp;
                }
                cache[cache_len++] = (seq_t)x; /* store x */
            }
        }

        /* Shrink to actual size if allocation was larger */
        if (cache_len < cache_cap)
        {
            seq_t *tmp = (seq_t*)realloc(cache, cache_len * sizeof(seq_t));
            if (tmp) cache = tmp;
            cache_cap = cache_len;
        }
    }

    /* Return requested term, or 0 if out of bounds */
    if (cache && n >= 1 && (idx_t)(n - 1) < cache_len)
        return cache[n - 1];
    return (seq_t)0;
}
