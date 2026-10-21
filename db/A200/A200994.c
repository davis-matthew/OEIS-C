/*
 * A200994  Triangular numbers, T(m), that are three-halves of another
 *          triangular number; T(m) such that 2*T(m) = 3*T(k) for some k.
 * https://oeis.org/A200994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=15, a(n)=98*a(n-1)-a(n-2)+15 for n>=2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;  /* current allocated length */

    /* Resize cache if needed */
    if (n + 1 > cache_len) {
        size_t new_len = (size_t)(n + 1);
        void *p = realloc(cache, new_len * sizeof(seq_t));
        if (!p) abort();                /* allocation failed */
        cache = (seq_t *)p;
        cache_len = new_len;
    }

    /* Initialise the first two values */
    cache[0] = (seq_t)0;
    if (cache_len > 1)
        cache[1] = (seq_t)15;

    /* Compute missing terms up to n */
    for (idx_t i = 2; i <= n; ++i)
        cache[i] = (seq_t)98 * cache[i-1] - cache[i-2] + (seq_t)15;

    return cache[n];
}
