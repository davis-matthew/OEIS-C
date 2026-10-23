/*
 * A192774  Coefficient of x^2 in the reduction of the n-th Fibonacci
 *          polynomial by x^3->x^2+2x+1.
 * https://oeis.org/A192774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+5*a(n-2)-a(n-3)-5*a(n-4)+a(n-5)+a(n-6); Initial
 *            values: a(1)=0, a(2)=0, a(3)=1, a(4)=1, a(5)=6, a(6)=10
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 27, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;        /* cache for computed values, index i holds a(i) */
    static idx_t  computed = 0;        /* number of elements currently allocated (size of cache) */

    if (n < computed)                  /* already computed */
        return cache[n];

    /* Extend the cached array to hold indices 0..n */
    idx_t new_size = n + 1;            /* we keep slot 0 unused */
    seq_t *tmp = (seq_t *)realloc((void *)cache, (size_t)(new_size * sizeof(seq_t)));
    if (tmp == NULL)
        return (seq_t)0;               /* allocation failure – not expected in tests */
    cache = tmp;

    /* Compute new terms */
    idx_t start = computed;            /* first index not yet computed */
    if (start == 0)                    /* first call */
        start = 1;

    for (idx_t i = start; i <= n; ++i) {
        if (i == 1)          cache[i] = (seq_t)0;
        else if (i == 2)     cache[i] = (seq_t)0;
        else if (i == 3)     cache[i] = (seq_t)1;
        else if (i == 4)     cache[i] = (seq_t)1;
        else if (i == 5)     cache[i] = (seq_t)6;
        else if (i == 6)     cache[i] = (seq_t)10;
        else {
            /* recurrence: a(i) = a(i-1)+5*a(i-2)-a(i-3)-5*a(i-4)+a(i-5)+a(i-6) */
            seq_t pos = cache[i-1];
            pos += (seq_t)5 * cache[i-2];
            pos += cache[i-5];
            pos += cache[i-6];

            seq_t neg = cache[i-3];
            neg += (seq_t)5 * cache[i-4];

            cache[i] = pos - neg;
        }
    }

    computed = new_size;   /* update size of the cached array */
    return cache[n];
}
