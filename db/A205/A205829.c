/*
 * A205829  Number of (n+1) X 2 0..2 arrays with the number of clockwise edge
 *          increases in every 2 X 2 subblock equal to two.
 * https://oeis.org/A205829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) – a(n-2) + a(n-3), a(1)=30, a(2)=111, a(3)=423
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 31, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_sz = 0;          /* size of allocated array (index >= cache_sz is invalid) */

    if (n < 1) return (seq_t)0;         /* sequence is defined only for n ≥ 1 */

    if (n >= cache_sz)                  /* need more terms */
    {
        idx_t old_sz = cache_sz;
        idx_t new_sz = n + 1;           /* allocate so that index n is reachable */

        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_sz);
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;

        /* initialise base terms if they are not yet set */
        if (new_sz > 1 && old_sz <= 1) cache[1] = (seq_t)30;
        if (new_sz > 2 && old_sz <= 2) cache[2] = (seq_t)111;
        if (new_sz > 3 && old_sz <= 3) cache[3] = (seq_t)423;

        idx_t start = old_sz ? old_sz : 1;
        for (idx_t i = start; i <= n; ++i)
        {
            if (i <= 3) continue; /* base already in place */
            seq_t v = cache[i-1] * (seq_t)4;
            v = v - cache[i-2];
            v = v + cache[i-3];
            cache[i] = v;
        }
        cache_sz = new_sz;
    }

    return cache[n];
}
