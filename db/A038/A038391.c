/*
 * A038391  Expansion of (x^3+2*x+1) / ((x-1)^4*(x^2+x+1)^2).
 * https://oeis.org/A038391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            2*a(n-1)-a(n-2)+2*a(n-3)-4*a(n-4)+2*a(n-5)-a(n-6)+2*a(n-7)-a(n-8)
 *            Initial values: a(0)=1, a(1)=4, a(2)=7, a(3)=13, a(4)=23,
 *            a(5)=33, a(6)=48, a(7)=69
 * Range:     u8 n <= 12, u16 n <= 93, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base[8] = {1, 4, 7, 13, 23, 33, 48, 69};

seq_t a(idx_t n)
{
    /* Return known initial values directly. */
    if (n < 8)
        return base[(size_t)n];

    /* Static cache to store previously computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Grow cache to hold up to index n. */
    if (n + 1 > cache_len)
    {
        size_t new_len = (size_t)(n + 1);
        /* realloc returns void* in both C and C++; cast to avoid C++ error. */
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!cache)
            abort();                      /* allocation failure */

        /* Compute terms from the current length up to n. */
        for (idx_t i = cache_len; i <= n; ++i)
        {
            if (i < 8)                     /* fill the initial segment if needed */
            {
                cache[(size_t)i] = base[(size_t)i];
                continue;
            }

            /* Recurrence: a(i) = 2*a(i-1)-a(i-2)+2*a(i-3)-4*a(i-4)+2*a(i-5)-a(i-6)+2*a(i-7)-a(i-8) */
            seq_t t1 = 2 * cache[(size_t)(i - 1)];
            seq_t t2 = 2 * cache[(size_t)(i - 3)];
            seq_t t3 = 2 * cache[(size_t)(i - 5)];
            seq_t t4 = 2 * cache[(size_t)(i - 7)];
            seq_t t5 = cache[(size_t)(i - 2)];
            seq_t t6 = 4 * cache[(size_t)(i - 4)];
            seq_t t7 = cache[(size_t)(i - 6)];
            seq_t t8 = cache[(size_t)(i - 8)];

            cache[(size_t)i] = (t1 + t2 + t3 + t4) - (t5 + t6 + t7 + t8);
        }
        cache_len = (idx_t)new_len;
    }

    return cache[(size_t)n];
}
