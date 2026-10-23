/*
 * A136128  Number of components in all permutations of [1,2,...,n].
 * https://oeis.org/A136128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..n-1} i! * (n-i)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are provided by the build environment. */
seq_t a(idx_t n)
{
    /* Offset 1: if n==0 return 0 to avoid UB */
    if (n == 0)
        return (seq_t)0;

    /* Static cache of factorials up to the largest n seen so far. */
    static idx_t max_n = 0;
    static seq_t *fact = NULL;

    /* Allocate the factorial array on first call. */
    if (fact == NULL) {
        fact = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (fact == NULL)
            return (seq_t)0;          /* allocation failure */
        fact[0] = (seq_t)1;
        max_n = 0;
    }

    /* Grow the factorial cache if necessary. */
    if (n > max_n) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(fact, new_size * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;          /* allocation failure */
        fact = tmp;
        for (idx_t i = max_n + 1; i <= n; ++i)
            fact[i] = fact[i - 1] * (seq_t)i;
        max_n = n;
    }

    /* Compute the sum of i! * (n-i)! for i = 0 .. n-1. */
    seq_t sum = (seq_t)0;
    for (idx_t i = 0; i < n; ++i)
        sum += fact[i] * fact[n - i];

    return sum;
}
