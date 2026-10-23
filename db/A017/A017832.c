/*
 * A017832  Expansion of 1/(1-x^4-x^5-x^6-x^7-x^8-x^9-x^10).
 * https://oeis.org/A017832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-4)+a(n-5)+a(n-6)+a(n-7)+a(n-8)+a(n-9)+a(n-10) (n>9)
 *            Initial values: a(0)=1, a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=1,
 *            a(6)=1, a(7)=1, a(8)=2, a(9)=3, a(10)=4
 * Range:     u8 n <= 24, u16 n <= 42, u32 n <= 78, u64 n <= 152,
 *            u128 n <= 298, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t   stored = 0;   /* highest index computed and stored in cache */

    /* initialize cache with the known first 11 terms */
    if (cache == NULL)
    {
        size_t init_cap = 11;     /* indices 0 .. 10 */
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        cap   = init_cap;

        cache[0] = (seq_t)1;
        cache[1] = cache[2] = cache[3] = (seq_t)0;
        cache[4] = cache[5] = cache[6] = cache[7] = (seq_t)1;
        cache[8] = (seq_t)2;
        cache[9] = (seq_t)3;
        cache[10] = (seq_t)4;
        stored = 10;
    }

    if (n <= stored)          /* already computed */
        return cache[n];

    /* Ensure enough space in the cache */
    if ((size_t)(n + 1) > cap)
    {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) /* allocation failed – nothing we can do */
            return (seq_t)0;
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = stored + 1; i <= n; ++i)
    {
        seq_t sum = 0;
        for (int k = 4; k <= 10; ++k)
            sum += cache[i - (idx_t)k];
        cache[i] = sum;
    }
    stored = n;
    return cache[n];
}
