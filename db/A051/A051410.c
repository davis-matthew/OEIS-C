/*
 * A051410  a(n+1) = a(n) + sum of digits of a(n)^3.
 * https://oeis.org/A051410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, less
 * Formula:   a(1) = 6, a(n) = a(n-1) + digit_sum( a(n-1)^3 )
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 45, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>            /* malloc, realloc */
#include "../utils/digit_sum.h" /* digit_sum(seq_t v) */

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache   = NULL;
    static idx_t  cap     = 0;    /* allocated size (bytes of seq_t) */
    static idx_t  last    = 0;    /* highest index computed (0 if none) */

    if (n == 0)
        return (seq_t)0;  /* undefined for n<1, but guard */

    /* ensure storage for at least indices 1..n */
    if (cap < n + 1) {
        size_t new_cap = cap > 0 ? cap : 1;
        while (new_cap <= n)
            new_cap <<= 1;                /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(EXIT_FAILURE);           /* out of memory */
        cache = tmp;
        cap   = (idx_t)new_cap;
    }

    /* initialize first term if not already done */
    if (last == 0) {
        cache[1] = (seq_t)6;  /* a(1) */
        last = 1;
    }

    /* compute successive terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t prev   = cache[i - 1];
        seq_t cube   = prev * prev;  /* prev^2 */
        cube *= prev;                /* prev^3 */
        seq_t ds     = digit_sum(cube);
        cache[i] = prev + ds;
    }

    last = n;
    return cache[n];
}
