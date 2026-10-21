/*
 * A087635  a(n) = S(n,3) where S(n,m) = Sum_{k=0..n}
 *          binomial(n,k)*Fibonacci(m*k).
 * https://oeis.org/A087635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 4*a(n-2), a(0)=0, a(1)=2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute OEIS sequence A087635 */
seq_t a(idx_t n)
{
    /* Static cache to save already computed terms across calls */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;       /* number of terms allocated */
    static idx_t last_computed = 0;    /* highest index that has a valid value */

    /* Expand cache if needed */
    if (n + 1 > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* allocation failed – safe return */
        cache      = tmp;
        cache_size = (idx_t)(n + 1);
    }

    /* Initialise base values if they are not already present */
    if (cache_size >= 1 && last_computed < 1) {
        cache[0]       = (seq_t)0;
        last_computed  = 0;
    }
    if (cache_size >= 2 && last_computed < 2) {
        cache[1]       = (seq_t)2;
        last_computed  = 1;
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t term = cache[i - 1];
        term *= (seq_t)6;
        term -= cache[i - 2] * (seq_t)4;
        cache[i] = term;
    }
    last_computed = n;

    return cache[n];
}
