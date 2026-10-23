/*
 * A167762  a(n) = 2*a(n-1)+3*a(n-2)-6*a(n-3) starting a(0)=a(1)=0, a(2)=1.
 * https://oeis.org/A167762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)+3*a(n-2)-6*a(n-3), with a(0)=a(1)=0, a(2)=1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t capacity = 0;    /* allocated array size                 */
    static idx_t max_computed = 0;/* largest index for which cache[i] is defined */

    /* initialise cache with the first three values */
    if (cache == NULL) {
        capacity = 3;                      /* indices 0,1,2 */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;       /* allocation failure */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        max_computed = 2;
    }

    /* already computed? */
    if (n <= max_computed)
        return cache[n];

    /* enlarge allocation if necessary */
    if (n + 1 > capacity) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failure */
        cache = tmp;
        capacity = n + 1;
    }

    /* compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] = 2 * cache[i - 1] + 3 * cache[i - 2] - 6 * cache[i - 3];
    }
    max_computed = n;

    return cache[n];
}
