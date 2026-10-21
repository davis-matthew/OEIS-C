/*
 * A060022  Expansion of (1-x-x^N)/((1-x)(1-x^2)(1-x^3)...(1-x^N)) for N = 3.
 * https://oeis.org/A060022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1)+a(n-2)-a(n-4)-a(n-5)+a(n-6) for n>5 initial
 *            values: a(0)=1, a(1)=0, a(2)=1, a(3)=0, a(4)=0, a(5)=-1
 * Range:     i8 n <= 40, i16 n <= 628, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base cases */
    static const seq_t base[6] = {1, 0, 1, 0, 0, -1};
    if (n < 6) return base[n];

    /* Allocate working array up to n */
    size_t len = (size_t)n + 1;
    seq_t *arr = (seq_t*)malloc(len * sizeof(seq_t));
    if (!arr) return 0;            /* fallback, should not happen */

    /* Set known initial values */
    arr[0] = 1;  arr[1] = 0;  arr[2] = 1;
    arr[3] = 0;  arr[4] = 0;  arr[5] = -1;

    /* Compute successive values using the recurrence */
    for (idx_t i = 6; i <= n; ++i)
    {
        arr[i] = arr[i-1] + arr[i-2] - arr[i-4] - arr[i-5] + arr[i-6];
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
