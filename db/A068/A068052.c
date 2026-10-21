/*
 * A068052  Start from 1, shift one left and sum mod 2 (bitwise-XOR) to get 3
 *          (11 in binary), then shift two steps left and XOR to get 15 (1111
 *          in binary), then three steps and XOR to get 119 (1110111 in
 *          binary), then four steps and so on.
 * https://oeis.org/A068052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 1; for n > 0, a(n) = a(n-1) XOR (2^n * a(n‑1))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation caches computed values to allow calls in any order. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* --- Cached sequence values ------------------------------------- */
    static seq_t *cache      = NULL;   /* array of computed terms     */
    static size_t cache_len  = 0;      /* number of elements filled   */
    static size_t cache_cap  = 0;      /* allocated capacity          */

    /* If the requested index is already cached, return it immediately. */
    if (n < cache_len)
        return cache[n];

    /* Ensure the cache has enough space for indices up to n (incl.). */
    size_t required = n + 1;
    if (required > cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap < required)
            new_cap <<= 1;

        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)  /* allocation failed – unlikely in the test harness   */
            return (seq_t)0;
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* --- Compute missing terms --------------------------------------- */
    /* The number of bits in the current seq_t type.  For arbitrary‑
       precision types this value is large enough; for fixed width types
       it keeps us from shifting by an illegal amount. */
    const size_t bits = sizeof(seq_t) * 8;

    for (size_t i = cache_len; i <= (size_t)n; ++i)
    {
        if (i == 0)
        {
            cache[0] = (seq_t)1;            /* a(0) = 1 */
        }
        else
        {
            /* Compute a(i) = a(i‑1) XOR (2^i * a(i‑1)) */
            seq_t mult;
            if (i < bits)
                mult = cache[i-1] << (idx_t)i;
            else
                mult = (seq_t)0;            /* shift too large – safe on 64‑bit */

            cache[i] = cache[i-1] ^ mult;
        }
    }

    cache_len = required;
    return cache[n];
}
