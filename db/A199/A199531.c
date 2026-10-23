/*
 * A199531  Number of -n..n arrays x(0..3) of 4 elements with zero sum and no
 *          two consecutive zero elements.
 * https://oeis.org/A199531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - a(n-4), with a(1)=12,
 *            a(2)=72, a(3)=212, a(4)=464
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial terms */
    static const seq_t init[4] = {12, 72, 212, 464};

    /* Static cache */
    static seq_t *cache    = NULL;
    static size_t cached   = 0;
    static size_t capacity = 0;

    if (n <= 4)
        return init[(size_t)n - 1];

    /* Initialise cache on first use */
    if (!cache)
    {
        capacity = 4;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        for (size_t i = 0; i < 4; ++i)
            cache[i] = init[i];
        cached = 4;
    }

    /* Extend cache until it contains a(n) */
    while (cached < (size_t)n)
    {
        if (cached == capacity)
        {
            capacity *= 2;
            cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
        }

        seq_t a1 = cache[cached - 1];
        seq_t a2 = cache[cached - 2];
        seq_t a3 = cache[cached - 3];
        seq_t a4 = cache[cached - 4];

        seq_t t1   = a1 * (seq_t)4;
        seq_t t2   = a3 * (seq_t)4;
        seq_t sum  = t1 + t2;
        seq_t t3   = a2 * (seq_t)6;
        seq_t sub  = t3 + a4;
        seq_t next = sum - sub;

        cache[cached++] = next;
    }

    return cache[n - 1];
}
