/*
 * A168871  Number of reduced words of length n in Coxeter group on 50
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^20 = I.
 * https://oeis.org/A168871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A168871:
 *  Recurrence (n >= 20):
 *      a(n) = 294 * a(n-20) - 12 * ( a(n-1)+a(n-2)+...+a(n-19) )
 *  Initial values:
 *      a(0) = 1
 *      a(n) = 50 * 49^(n-1)          for 1 <= n <= 19
 *
 *  The function a() returns the nth term for any n >= 0.
 *  It uses only integer arithmetic and O(n) time.
 */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are defined by the build environment. */
seq_t a(idx_t n)
{
    /* Handle the trivial case. */
    if (n == 0)
        return (seq_t)1;

    /* For 1 <= n <= 19: use the explicit formula. */
    if (n < 20)
    {
        seq_t pow = (seq_t)1;
        for (idx_t i = 1; i < n; ++i)
            pow *= (seq_t)49;      /* compute 49^(n-1) */
        return (seq_t)50 * pow;   /* 50 * 49^(n-1) */
    }

    /* For n >= 20: compute iteratively using the recurrence. */
    /* Allocate an array of a[0..n].  size_t is large enough for n <= 10000. */
    size_t len = (size_t)n + 1;
    seq_t *arr = (seq_t *)malloc(len * sizeof(seq_t));
    if (!arr)
        return (seq_t)0;  /* allocation failure */

    /* Initialise first 20 terms. */
    arr[0] = (seq_t)1;            /* a(0) */
    seq_t pow = (seq_t)1;
    for (idx_t i = 1; i <= 19; ++i)    /* i runs from 1 to 19 */
    {
        pow *= (seq_t)49;            /* pow = 49^i */
        arr[i] = (seq_t)50 * pow;    /* a(i) = 50 * 49^(i-1) */
    }

    /* Compute a(n) for n >= 20. */
    for (idx_t i = 20; i <= n; ++i)
    {
        seq_t sum = (seq_t)0;
        for (idx_t j = 1; j <= 19; ++j)
            sum += arr[i - j];                /* a(i-1)+...+a(i-19) */

        /* a(i) = 294*a(i-20) - 12*sum */
        arr[i] = (seq_t)294 * arr[i - 20] - (seq_t)12 * sum;
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
