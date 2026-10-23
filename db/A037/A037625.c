/*
 * A037625  Base 4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,3,0.
 * https://oeis.org/A037625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=2; a(n+1)=4*a(n)+p(n) where p(n)=2,3,0 for n mod 3 = 0,1,2
 *            respectively
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t _base4_pattern[3] = { (seq_t)2, (seq_t)3, (seq_t)0 };

seq_t a(idx_t n)
{
    /* Cache the computed sequence values to serve repeated calls efficiently. */
    /* cache[0] is unused to allow 1‑based indexing. */
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;   /* largest index stored in cache */

    if (n == 0) return 0;          /* safety, the sequence starts at index 1 */

    /* First call: allocate cache exactly large enough for n. */
    if (cache == NULL)
    {
        cached_len = n;
        cache = (seq_t *)malloc((cached_len + 1) * sizeof(seq_t));
        if (!cache) return 0;     /* out‑of‑memory fallback */

        cache[1] = (seq_t)2;     /* a(1) = 2 */
        for (idx_t i = 2; i <= n; ++i)
        {
            seq_t d = _base4_pattern[(i - 1) % 3];
            cache[i] = cache[i - 1] * (seq_t)4 + d;
        }
        return cache[n];
    }

    /* If the requested value is already cached, return it. */
    if (n <= cached_len)
        return cache[n];

    /* Grow the cache until it can hold index n. */
    idx_t new_len = cached_len;
    while (new_len < n)
        new_len <<= 1;           /* double size */

    seq_t *new_cache = (seq_t *)realloc(cache, (new_len + 1) * sizeof(seq_t));
    if (!new_cache) return 0;     /* out‑of‑memory fallback */
    cache = new_cache;

    /* Compute the new entries on top of the existing ones. */
    for (idx_t i = cached_len + 1; i <= n; ++i)
    {
        seq_t d = _base4_pattern[(i - 1) % 3];
        cache[i] = cache[i - 1] * (seq_t)4 + d;
    }
    cached_len = n;
    return cache[n];
}
