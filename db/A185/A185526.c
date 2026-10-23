/*
 * A185526  Number of (n+2) X 3 binary arrays with each 3 X 3 subblock
 *          nonsingular.
 * https://oeis.org/A185526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-2) + 12*a(n-3) with initial values
 *            a(1)=174, a(2)=726, a(3)=3030
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 28, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* cache[0] unused, cache[1..] holds values  */
    static size_t cap = 0;          /* allocated capacity  */
    static idx_t  cached_up_to = 0; /* largest n computed so far  */

    /* initial allocation and seeds – executed only on first call */
    if (cache == NULL) {
        cap = 4; /* indices 0 … 3  */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL)        /* allocation failure – cannot compute */
            return 0;
        cache[0] = (seq_t)0;      /* unused but defined for safety   */
        cache[1] = (seq_t)174;
        cache[2] = (seq_t)726;
        cache[3] = (seq_t)3030;
        cached_up_to = 3;
    }

    /* grow array if required – will reallocate only when needed   */
    if ((size_t)n >= cap) {
        size_t new_cap = cap * 2;
        while ((size_t)n >= new_cap)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)            /* allocation failure – cannot compute */
            return 0;
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms up to requested n   */
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        seq_t val = 3 * cache[i - 1] + 2 * cache[i - 2] + 12 * cache[i - 3];
        cache[i] = val;
    }
    if (n > cached_up_to)
        cached_up_to = n;

    /* for n == 0 (outside of OEIS offset) return 0  */
    if (n == 0)
        return (seq_t)0;
    return cache[n];
}
