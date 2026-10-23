/*
 * A122265  10th-order Fibonacci numbers: a(n+1) = a(n)+...+a(n-9) with a(0) =
 *          ... = a(8) = 0, a(9) = 1.
 * https://oeis.org/A122265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+...+a(n-10) for n>=10; a(0) = a(1) = ... =
 *            a(8) = 0, a(9) = 1
 * Range:     u8 n <= 17, u16 n <= 26, u32 n <= 42, u64 n <= 74, u128 n <= 138,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static memoisation state */
    static seq_t *cache = NULL;
    static size_t  cap      = 0;
    static idx_t   max_comp = 0;   /* highest index computed */
    static seq_t   sum_last = 0;   /* sum of last 10 terms up to max_comp */

    /* Initialisation on first call */
    if (cache == NULL) {
        /* Allocate at least 10 slots (indices 0..9) */
        size_t init_cap = (n > 9) ? (size_t)(n + 1) : 10;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        /* Base values */
        for (idx_t i = 0; i < 10; ++i) {
            cache[i] = (i == 9 ? (seq_t)1 : (seq_t)0);
        }
        max_comp = 9;
        sum_last = (seq_t)1;   /* sum of cache[0]..cache[9] */
        cap      = init_cap;
    }

    /* Resize cache if needed */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap   = new_cap;
    }

    /* Fast return for already computed indices */
    if (n <= max_comp) {
        if (n <= 9) {
            return (n == 9) ? (seq_t)1 : (seq_t)0;
        }
        return cache[n];
    }

    /* Compute forward from max_comp+1 up to n */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        cache[i] = sum_last;
        /* Update sliding sum for next step */
        sum_last = sum_last + cache[i];
        if (i >= 10) {
            sum_last = sum_last - cache[i - 10];
        }
    }

    max_comp = n;
    return cache[n];
}
