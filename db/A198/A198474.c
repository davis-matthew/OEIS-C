/*
 * A198474  Number of 2n X 2 0..2 arrays with values 0..2 introduced in row
 *          major order and each element equal to an odd number of horizontal
 *          and vertical neighbors.
 * https://oeis.org/A198474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 16*a(n-2), a(1)=2, a(2)=12
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n must be >= 1.  n==0 is not part of the sequence and is handled safely. */
    if (n == 0)
        return 0;

    static seq_t *arr = NULL;
    static idx_t max_n = 0;

    if (arr == NULL) {                         /* First call: create the array.     */
        size_t sz = (size_t)n + 1;
        arr = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!arr)
            abort();                           /* Out of memory.                    */

        arr[0] = 0;                             /* Dummy entry; not used.           */
        if (n >= 1) arr[1] = 2;
        if (n >= 2) arr[2] = 12;

        for (idx_t i = 3; i <= n; ++i)
            arr[i] = 9 * arr[i-1] - 16 * arr[i-2];

        max_n = n;
        return arr[n];
    }

    /* Extend the cache if a larger n is requested. */
    if (n > max_n) {
        size_t sz = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(arr, sz * sizeof(seq_t));
        if (!tmp)
            abort();                           /* Out of memory.                    */
        arr = tmp;

        /* Ensure base terms are correct after reallocation. */
        if (max_n < 1 && n >= 1)
            arr[1] = 2;
        if (max_n < 2 && n >= 2)
            arr[2] = 12;

        /* Start recurrence from the next uncomputed index. */
        idx_t start = (max_n >= 2) ? max_n + 1 : 3;
        for (idx_t i = start; i <= n; ++i)
            arr[i] = 9 * arr[i-1] - 16 * arr[i-2];

        max_n = n;
    }

    return arr[n];
}
