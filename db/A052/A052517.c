/*
 * A052517  Number of ordered pairs of cycles over all n-permutations having
 *          two cycles.
 * https://oeis.org/A052517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*(n-1)!*H_{n-1} for n>=2, a(0)=a(1)=0 Recurrence used
 *            for efficient computation: a(2) = 2 a(k+2) = a(k+1)*(2*k+1) -
 *            a(k)*k*k for k >= 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cached values  */
    static idx_t size   = 0;          /* amount of cached values */

    /* initialise cache with first three terms */
    if (!cache) {
        /* allocate space for terms 0,1,2 */
        cache = (seq_t*)malloc(3 * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 2;
        size    = 3;
    }

    /* return already computed value if possible */
    if (n < size)
        return cache[n];

    /* extend cache up to term n using the recurrence */
    cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
    for (idx_t i = size; i <= n; ++i) {
        if (i == 2) {            /* term already set, skip */
            cache[i] = 2;
            continue;
        }
        /* For term i (>=3) use recurrence with k = i-2 */
        idx_t k = i - 2;
        seq_t term = cache[k+1] * (seq_t)(2*k + 1) - cache[k] * (seq_t)(k*k);
        cache[i] = term;
    }
    size = n + 1;
    return cache[n];
}
