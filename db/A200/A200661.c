/*
 * A200661  Number of 0..1 arrays x(0..n-1) of n elements with each no smaller
 *          than the sum of its three previous neighbors modulo 2.
 * https://oeis.org/A200661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)-a(n-5)-a(n-6)+a(n-7) with a(0)=1, a(1)=2,
 *            a(2)=3, a(3)=5, a(4)=8, a(5)=12, a(6)=17. Offset: 1 (a(1) is the
 *            first term)
 * Range:     u8 n <= 13, u16 n <= 30, u32 n <= 62, u64 n <= 128,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* cache[0]..cache[cache_size-1] */
    static idx_t  size    = 0;      /* capacity of cache */
    static idx_t  last    = 0;      /* largest index already computed */

    if (n < 1) return 0;            /* undefined for n<1 */

    /* enlarge cache if necessary */
    if (size <= n) {
        idx_t new_size = n + 1;         /* need indices 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;             /* out‑of‑memory */
        cache = tmp;
        for (idx_t i = size; i < new_size; ++i)
            cache[i] = 0;                /* initialize new slots */
        size = new_size;
    }

    /* ensure the base case a(0)=1 is stored */
    cache[0] = 1;

    /* compute missing values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 1)       cache[1] = 2;
        else if (i == 2)  cache[2] = 3;
        else if (i == 3)  cache[3] = 5;
        else if (i == 4)  cache[4] = 8;
        else if (i == 5)  cache[5] = 12;
        else if (i == 6)  cache[6] = 17;
        else              cache[i] = cache[i-1] + cache[i-2]
                               - cache[i-5] - cache[i-6]
                               + cache[i-7];
    }

    last = n;
    return cache[n];
}
