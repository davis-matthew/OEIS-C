/*
 * A163391  Number of reduced words of length n in Coxeter group on 9
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^5 = I.
 * https://oeis.org/A163391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 7*a(n-1)+7*a(n-2)+7*a(n-3)+7*a(n-4)-28*a(n-5) * a(0)=1,
 *            a(1)=9, a(2)=72, a(3)=576, a(4)=4608, a(5)=36828
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first six terms explicitly. */
    static const seq_t init[6] = {
        (seq_t)1,      /* a(0) */
        (seq_t)9,      /* a(1) */
        (seq_t)72,     /* a(2) */
        (seq_t)576,    /* a(3) */
        (seq_t)4608,   /* a(4) */
        (seq_t)36828   /* a(5) */
    };

    if (n < 6u) /* n is unsigned; direct comparison is safe */
        return init[(size_t)n];

    /* Allocate a temporary array to hold the values up to n.
     * The array is unsigned; its size is n+1 which fits in size_t.
     */
    seq_t *arr = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!arr) return (seq_t)0;                /* allocation failed */

    /* Initialize the first six values. */
    for (size_t i = 0; i < 6; ++i)
        arr[i] = init[i];

    /* Compute successive terms using the linear recurrence. */
    for (idx_t i = 6; i <= n; ++i)
    {
        /* Since all operands are of type seq_t, no overflow is
         * introduced beyond the natural size of seq_t.
         */
        arr[i] = (seq_t)7 * (arr[i - 1] + arr[i - 2] + arr[i - 3] + arr[i - 4])
               - (seq_t)28 * arr[i - 5];
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
