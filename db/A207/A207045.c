/*
 * A207045  Number of (n+1) X 4 0..2 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly one clockwise edge increases.
 * https://oeis.org/A207045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 11, u32 n <= 33, u64 n <= 77, u128 n <= 164,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 4*a(n-3) + a(n-4)  for n > 11
 * with initial values:
 *   a(1) = 441, a(2) = 552, a(3) = 750, a(4) = 1272,
 *   a(5) = 2094, a(6) = 3408, a(7) = 5850, a(8) = 9792,
 *   a(9) = 15966, a(10) = 26880, a(11) = 45024
 */

#include <stdlib.h>   /* malloc, free */

seq_t a(idx_t n)
{
    /* Initial terms (index 1..11).  arr[0] is unused. */
    static const seq_t init[12] = {
        0,          /* placeholder for index 0 */
        441, 552, 750, 1272, 2094, 3408, 5850, 9792,
        15966, 26880, 45024
    };

    if (n <= 11)
        return init[n];

    /* Allocate array [0..n] */
    seq_t *arr = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!arr)
        return 0;          /* Out‑of‑memory fallback */

    /* Copy initial values */
    for (idx_t i = 1; i <= 11; ++i)
        arr[i] = init[i];

    /* Compute forward using the recurrence */
    for (idx_t i = 12; i <= n; ++i)
    {
        arr[i] = (seq_t)4 * arr[i - 3] + arr[i - 4];
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
