/*
 * A052625  E.g.f. (1-x)^2/(1-2x+x^2-x^3).
 * https://oeis.org/A052625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=0, a(2)=0, and for n≥0 * a(n+3) =
 *            (11n+6+n³+6n²)·a(n) - (n²+5n+6)·a(n+1) + (2n+6)·a(n+2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* initialise with first three terms */
    if (cache_size == 0)
    {
        cache_size = 3;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 0;
        cache[2] = 0;
    }

    /* extend cache up to n */
    if ((size_t)n >= cache_size)
    {
        size_t new_size = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

        for (size_t i = cache_size; i <= n; ++i)
        {
            /* first three positions already set */
            if (i <= 2) continue;

            unsigned long long k = i - 3;                 /* index in recurrence */
            unsigned long long k2 = k * k;
            /* compute k^3 safely with 128‑bit intermediate */
            unsigned long long k3 = (unsigned long long)((__int128)k2 * k);

            unsigned long long c1 = 11ULL * k + 6ULL + k3 + 6ULL * k2;
            unsigned long long c2 = k2 + 5ULL * k + 6ULL;
            unsigned long long c3 = 2ULL * k + 6ULL;

            seq_t prod1 = cache[k];
            prod1 *= (seq_t)c1;          /* C1 * a(k)      */
            seq_t prod2 = cache[k + 2];
            prod2 *= (seq_t)c3;          /* C3 * a(k+2)    */
            seq_t prod3 = cache[k + 1];
            prod3 *= (seq_t)c2;          /* C2 * a(k+1)    */

            seq_t val = prod1;
            val += prod2;                /* +C3*a(k+2)     */
            val -= prod3;                /* -C2*a(k+1)     */

            cache[i] = val;
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
