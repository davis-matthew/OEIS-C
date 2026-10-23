/*
 * A108096  Coefficients of square root of theta series of D_4 (see A004011).
 * https://oeis.org/A108096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   The sequence a(n) satisfies (A(q))^2 = 1 + Sum_{m>=1} 24 *
 *            Sum_{d|m, d odd} d * q^{2m}, where A(q)=Sum_{n>=0} a(n) q^{2n}.
 *            Therefore, for n>=1: 2*a(n) + Sum_{k=1}^{n-1} a(k) * a(n-k) = 24
 *            * Sum_{d|n, d odd} d which gives a(n) = ( 24*Sum_{d|n, d odd} d
 *            - Sum_{k=1}^{n-1} a(k)*a(n-k) ) / 2
 * Range:     i8 n <= 1, i16 n <= 4, i32 n <= 8, i64 n <= 15, i128 n <= 29,
 *            big any n
 * Helpers:   odd_divisor_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/odd_divisor_sum.h"

seq_t a(idx_t n)
{
    /* static storage for the sequence */
    static seq_t *arr = NULL;
    static idx_t max_computed = 0;   /* last index that has been computed */
    static idx_t capacity   = 0;      /* allocated capacity */

    /* initialise with a(0)=1 */
    if (arr == NULL)
    {
        capacity = 1;
        arr = (seq_t *)malloc(sizeof(seq_t) * (capacity + 1));
        if (!arr) exit(1);
        arr[0] = (seq_t)1;
        max_computed = 0;
    }

    /* increase capacity if necessary */
    if (n > capacity)
    {
        idx_t new_cap = capacity ? capacity : 1;
        while (new_cap < n) new_cap <<= 1;
        seq_t *new_arr = (seq_t *)malloc(sizeof(seq_t) * (new_cap + 1));
        if (!new_arr) exit(1);
        memcpy(new_arr, arr, sizeof(seq_t) * (max_computed + 1));
        for (idx_t i = max_computed + 1; i <= new_cap; ++i) new_arr[i] = 0;
        free(arr);
        arr = new_arr;
        capacity = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        /* t_even[i] = 24 * sum_{d|i, d odd} d */
        seq_t odd_sum = odd_divisor_sum(i);
        seq_t t_even = (seq_t)24 * odd_sum;

        /* compute Σ_{k=1}^{i-1} a(k)*a(i-k) efficiently */
        idx_t half = i / 2;
        seq_t sum_half = 0;
        for (idx_t k = 1; k <= (i - 1) / 2; ++k)
            sum_half += arr[k] * arr[i - k];

        seq_t full_sum;
        if (i % 2 == 0 && half >= 1)
        {
            seq_t mid = arr[half] * arr[half];
            full_sum = sum_half * 2 + mid;
        }
        else
        {
            full_sum = sum_half * 2;
        }

        /* a[i] = (t_even - full_sum) / 2 */
        arr[i] = (t_even - full_sum) / (seq_t)2;
    }

    max_computed = n;
    return arr[n];
}
