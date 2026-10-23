/*
 * A102042  a(n) = a(n-1) + last digit of a(n-1), starting at 9.
 * https://oeis.org/A102042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = a(n-1) + (a(n-1) mod 10), a(1) = 9
 * Range:     u8 n <= 49, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t stored = 0;          /* number of terms stored in cache (index into cache is n-1) */

    /* generate terms up to n if necessary */
    while (stored < n)
    {
        if (stored == 0)
        {   /* first term */
            seq_t val = 9;
            seq_t *tmp;
            /* grow cache from 0 to 1 */
            tmp = (seq_t*)realloc(cache, sizeof(seq_t));
            if (!tmp);
            cache = tmp;
            cache[0] = val;
            stored = 1;
        }
        else
        {
            seq_t prev = cache[stored - 1];
            seq_t val  = prev + (prev % 10);
            seq_t *tmp;
            /* grow cache by one element */
            tmp = (seq_t*)realloc(cache, sizeof(seq_t) * (stored + 1));
            if (!tmp);
            cache = tmp;
            cache[stored] = val;
            stored++;
        }
    }
    return cache[n - 1];
}
