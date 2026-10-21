/*
 * A099560  a(n) = Sum_{k=0..floor(n/3)} C(n-2k,k-1).
 * https://oeis.org/A099560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-3) - a(n-4) - a(n-6) Initial values:
 *            a(0)=0, a(1)=0, a(2)=0, a(3)=1, a(4)=1, a(5)=1
 * Range:     u8 n <= 17, u16 n <= 32, u32 n <= 61, u64 n <= 119,
 *            u128 n <= 235, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* grow cache if necessary */
    if (n >= cache_len)
    {
        idx_t new_len = n + 1;
        /* realloc returns void*, cast to seq_t* for C++ compatibility */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – return 0 as a safe fallback */
            return 0;
        }
        cache = tmp;

        for (idx_t i = cache_len; i < new_len; ++i)
        {
            if (i == 0 || i == 1 || i == 2) {
                cache[i] = 0;
            } else if (i == 3 || i == 4 || i == 5) {
                cache[i] = 1;
            } else {
                /* recurrence: a(n) = a(n-1) + 2*a(n-3) - a(n-4) - a(n-6) */
                seq_t t = cache[i - 1];
                t += cache[i - 3];
                t += cache[i - 3];    /* 2 * a(n-3) */
                t -= cache[i - 4];
                t -= cache[i - 6];
                cache[i] = t;
            }
        }
        cache_len = new_len;
    }
    return cache[n];
}
