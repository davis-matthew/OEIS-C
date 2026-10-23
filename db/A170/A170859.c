/*
 * A170859  a(0) = 1, a(1) = 0; a(2^i + j) = 3a(j) + 2a(j + 1) for 0 <= j <
 *          2^i.
 * https://oeis.org/A170859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A170859
   a(0) = 1
   a(1) = 0
   a(2^i + j) = 3*a(j) + 2*a(j + 1)   for 0 <= j < 2^i
*/

#include <stdlib.h>          /* malloc, free */

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    /* allocate array [0 .. n] */
    seq_t *arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!arr) return (seq_t)0;          /* out of memory: return 0 */

    arr[0] = (seq_t)1;
    arr[1] = (seq_t)0;

    idx_t pow2 = 2;                     /* 2^1, first block that uses the recurrence */
    while (pow2 <= n) {
        /* number of elements to compute in this block */
        idx_t max_j = pow2 - 1;
        if (pow2 + max_j > n) max_j = n - pow2;

        for (idx_t j = 0; j <= max_j; ++j) {
            /* arr[pow2 + j] = 3*arr[j] + 2*arr[j+1] */
            arr[pow2 + j] = (seq_t)3 * arr[j] + (seq_t)2 * arr[j + 1];
        }
        pow2 <<= 1;                     /* next power of two */
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
