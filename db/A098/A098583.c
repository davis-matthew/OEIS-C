/*
 * A098583  Expansion of (1+2*x+4*x^2+8*x^3+16*x^4)/(1-x-32*x^6).
 * https://oeis.org/A098583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 32*a(n-6) for n ≥ 6 with a(0)=1, a(1)=3, a(2)=7,
 *            a(3)=15, a(4)=31, a(5)=31 (offset 0). This implements the
 *            expansion of (1+2x+4x^2+8x^3+16x^4)/(1‑x‑32x^6)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Grow cache if necessary */
    if (n + (idx_t)1 > cache_len)
    {
        idx_t new_len = n + (idx_t)1;
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
        if (!new_cache) return 0;        /* memory allocation failed   */
        cache = new_cache;

        /* Initialize newly allocated entries */
        for (idx_t i = cache_len; i < new_len; ++i)
        {
            if (i == 0)          cache[i] = (seq_t)1;
            else if (i == 1)      cache[i] = (seq_t)3;
            else if (i == 2)      cache[i] = (seq_t)7;
            else if (i == 3)      cache[i] = (seq_t)15;
            else if (i == 4)      cache[i] = (seq_t)31;
            else if (i == 5)      cache[i] = (seq_t)31;
            else                 cache[i] = cache[i-1] + (seq_t)32 * cache[i-6];
        }
        cache_len = new_len;
    }
    return cache[n];
}
