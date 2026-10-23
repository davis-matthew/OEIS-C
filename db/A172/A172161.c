/*
 * A172161  Greedy Coppersmith-Winograd sequence.
 * https://oeis.org/A172161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((∑_{i=1}^{n-1} a(i))/2) + 1 for n > 4, with initial
 *            values a(1)=0, a(2)=1, a(3)=2, a(4)=3, a(5)=4
 * Range:     u8 n <= 13, u16 n <= 27, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static caches for a(n) and cumulative sums */
    static seq_t *a_cache = NULL;
    static seq_t *sum_cache = NULL;
    static idx_t max_cache = 0;   /* highest n that has been computed */

    /* Initialize cache for the first 5 terms if needed */
    if (a_cache == NULL)
    {
        /* allocate space for indices 0 .. 5  (6 entries) */
        a_cache   = (seq_t *)malloc(6 * sizeof(seq_t));
        sum_cache = (seq_t *)malloc(6 * sizeof(seq_t));

        /* initial values (a[0] unused) */
        a_cache[0] = (seq_t)0;   /* placeholder */
        a_cache[1] = (seq_t)0;
        a_cache[2] = (seq_t)1;
        a_cache[3] = (seq_t)2;
        a_cache[4] = (seq_t)3;
        a_cache[5] = (seq_t)4;

        /* cumulative sums: sum[0] = 0 */
        sum_cache[0] = (seq_t)0;
        sum_cache[1] = a_cache[1];
        sum_cache[2] = sum_cache[1] + a_cache[2];
        sum_cache[3] = sum_cache[2] + a_cache[3];
        sum_cache[4] = sum_cache[3] + a_cache[4];
        sum_cache[5] = sum_cache[4] + a_cache[5];

        max_cache = 5;
    }

    /* Return cached value if already computed */
    if (n <= max_cache)
        return a_cache[n];

    /* Ensure the arrays are large enough to hold up to n */
    if (sum_cache = (seq_t *)realloc(sum_cache, (size_t)(n + 1) * sizeof(seq_t)))
        a_cache = (seq_t *)realloc(a_cache,   (size_t)(n + 1) * sizeof(seq_t));
    else
    {
        /* realloc failed – in practice this should not happen for the limits */
        return (seq_t)0;
    }

    /* Compute terms from max_cache+1 up to n */
    for (idx_t i = max_cache + 1; i <= n; ++i)
    {
        seq_t prev_sum = sum_cache[i - 1];      /* sum_{1}^{i-1} a(j) */
        seq_t ai = prev_sum / (seq_t)2 + (seq_t)1; /* recurrence */
        a_cache[i] = ai;
        sum_cache[i] = prev_sum + ai;
    }

    max_cache = n;
    return a_cache[n];
}
