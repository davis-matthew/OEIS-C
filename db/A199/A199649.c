/*
 * A199649  Number of n X 2 0..2 arrays with values 0..2 introduced in row
 *          major order and each element equal to no more than two horizontal
 *          and vertical neighbors.
 * https://oeis.org/A199649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2, a(2)=14, a(3)=113, a(4)=953, a(5)=8037, a(6)=67774; for
 *            n>6: a(n) = 8*a(n-1) + 4*a(n-2) - 2*a(n-3) - 8*a(n-4)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */

seq_t a(idx_t n)
{
    if (n == 0)     /* sequence is defined for n ≥ 1 */
        return (seq_t)0;

    /* Static cache of computed values, indexed 1..max */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;          /* current capacity (>= n+1) */
    static idx_t   computed = 0;       /* largest n computed so far */

    /* Grow capacity if necessary */
    if (n >= (idx_t)cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 16) new_cap = 16;      /* small initial size */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap   = new_cap;
        /* No initialization of new space needed – values will be written later */
    }

    /* Compute missing terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)2;
        } else if (i == 2) {
            cache[2] = (seq_t)14;
        } else if (i == 3) {
            cache[3] = (seq_t)113;
        } else if (i == 4) {
            cache[4] = (seq_t)953;
        } else if (i == 5) {
            cache[5] = (seq_t)8037;
        } else if (i == 6) {
            cache[6] = (seq_t)67774;
        } else {
            /* Recurrence for n > 6 */
            cache[i] = cache[i-1] * (seq_t)8
                     + cache[i-2] * (seq_t)4
                     - cache[i-3] * (seq_t)2
                     - cache[i-4] * (seq_t)8;
        }
    }
    computed = (computed > n) ? computed : n;
    return cache[n];
}
