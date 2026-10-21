/*
 * A221636  Odd numbers n with the property that Collatz (3x+1) trajectory of
 *          n contains exactly four terms that are divisible by 5.
 * https://oeis.org/A221636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = first odd n whose Collatz trajectory contains exactly
 *            four terms divisible by 5. (OEIS A221636)
 * Range:     u8 n <= 8, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache of already found terms */
    static seq_t *cache   = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;

    /* last odd number that was examined */
    static seq_t last_odd = 1;  /* start from 1, next test will be 3 */

    if (n == 0)                /* index 0 is unused, should not happen */
        return (seq_t)0;

    /* extend cache until we have at least n terms */
    while (cache_size < (size_t)n)
    {
        /* examine the next odd number */
        last_odd += (seq_t)2;          /* 1->3->5->7... */

        seq_t x = last_odd;
        seq_t t = x;
        int cnt = 0;                   /* # of terms divisible by 5 */

        /* Simulate Collatz trajectory */
        while (t != (seq_t)1)
        {
            if (t % (seq_t)5 == (seq_t)0)
                ++cnt;

            if (t & (seq_t)1)          /* odd */
                t = (seq_t)(3 * t + (seq_t)1);
            else                       /* even */
                t >>= 1;
        }
        /* t == 1; 1 is not divisible by 5 */

        if (cnt == 4)                 /* a term of the sequence */
        {
            if (cache_size == cache_cap)
            {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = x;
        }
    }

    return cache[n - 1];
}
