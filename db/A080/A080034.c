/*
 * A080034  a(n) is taken to be the smallest positive integer not already
 *          present which is consistent with the condition "n is a member of
 *          the sequence if and only if a(n) is congruent to 3 mod 4".
 * https://oeis.org/A080034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A080034
 *
 *  The sequence satisfies the linear recurrence
 *      a(n) = 2*a(n-8) - a(n-16)   for n > 15
 *  with the initial values
 *      a(0)=1,  a(1)=3,  a(2)=4,  a(3)=7,  a(4)=11,
 *      a(5)=6,  a(6)=15, a(7)=19, a(8)=9,  a(9)=23,
 *      a(10)=12, a(11)=27, a(12)=31, a(13)=14, a(14)=35, a(15)=39.
 *
 *  The function caches the values so that it works for arbitrary
 *  sequences of calls.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values for 0 ≤ n < 16 */
    static const seq_t init[16] = {
        1, 3, 4, 7, 11, 6, 15, 19,
        9, 23, 12, 27, 31, 14, 35, 39
    };

    /* Cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cached_size = 0;          /* number of elements currently stored */

    if (n < 16)
        return init[(size_t)n];

    size_t required = (size_t)n + 1;

    /* Allocate or grow the cache if necessary */
    if (cached_size < required)
    {
        /* Remember the old size before reallocating */
        size_t old_size = cached_size;

        seq_t *new_cache = (seq_t *)realloc(cache, required * sizeof(seq_t));
        if (!new_cache)
            abort();   /* allocation failure */

        cache = new_cache;
        cached_size = required;

        /* On first allocation copy the initial 16 terms */
        if (old_size == 0)
        {
            for (size_t i = 0; i < 16; ++i)
                cache[i] = init[i];
        }

        /* Compute new terms using the recurrence */
        size_t start = old_size > 16 ? old_size : 16;
        for (size_t i = start; i <= n; ++i)
            cache[i] = 2 * cache[i - 8] - cache[i - 16];
    }

    return cache[(size_t)n];
}
