/*
 * A025270  a(n) = a(1)*a(n-1) + a(2)*a(n-2) + ...+ a(n-1)*a(1) for n >= 5,
 *          with initial values 1,2,1,1.
 * https://oeis.org/A025270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2, a(3)=1, a(4)=1,
 *            a(n)=a(1)*a(n-1)+a(2)*a(n-2)+...+a(n-1)*a(1) for n ≥ 5
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* External linkage as required */
seq_t a(idx_t n)
{
    /* static storage for the sequence */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* capacity of the cache (number of slots)   */
    static size_t known = 0;   /* highest index for which cache[index] is valid */

    if (n == 0) /* offset is 1; n==0 is not used but guard anyway */
        return 0;

    /* Ensure the array has room for index n */
    if ((size_t)n + 1 > cap) {
        size_t newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        cache = tmp;
        /* zero the new part so that products are correct   */
        for (size_t i = cap; i < newcap; ++i)
            cache[i] = 0;
        cap = newcap;
    }

    /* If we already computed up to n, just return the cached value */
    if ((size_t)n <= known)
        return cache[n];

    /* Compute new terms, starting just beyond the largest computed index */
    for (size_t i = known + 1; i <= (size_t)n; ++i) {
        if (i == 1)
            cache[i] = 1;
        else if (i == 2)
            cache[i] = 2;
        else if (i == 3 || i == 4)
            cache[i] = 1;
        else {
            seq_t sum = 0;
            size_t half = (i - 1) / 2;                /* floor((i-1)/2) */
            for (size_t k = 1; k <= half; ++k)
                sum += cache[k] * cache[i - k];       /* compute one pair */
            sum += sum;                               /* double the pairs   */
            if ((i & 1) == 0)                         /* i even: middle term once */
                sum += cache[i / 2] * cache[i / 2];
            cache[i] = sum;
        }
    }

    known = (size_t)n;
    return cache[n];
}
