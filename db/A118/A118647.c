/*
 * A118647  a(n) is the number of binary strings of length n such that no
 *          subsequence of length 4 contains 3 or more ones.
 * https://oeis.org/A118647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-4) - a(n-6) with initial values
 *            a(1)=2, a(2)=4, a(3)=7, a(4)=11, a(5)=19, a(6)=33
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 40, u64 n <= 81, u128 n <= 164,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Bounds below the offset are undefined – return 0 */
    if (n < 1u)
        return 0;

    /* Base values for indices 1..6 */
    static const seq_t base_vals[7] = {0, 2, 4, 7, 11, 19, 33};

    /* For very small n simply return the known value */
    if (n <= 6u)
        return base_vals[n];

    /* Allocate an array to hold values up to n.
       Since n ≤ 10 000 in the typical tests this is inexpensive. */
    seq_t *arr = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!arr)
        return 0; /* Allocation failed – behaviour unspecified */

    /* Fill the base values */
    for (idx_t i = 1; i <= 6; ++i)
        arr[i] = base_vals[i];

    /* Compute subsequent terms using the recurrence */
    for (idx_t i = 7; i <= n; ++i)
        arr[i] = arr[i - 1] + arr[i - 2] + arr[i - 4] - arr[i - 6];

    seq_t result = arr[n];
    free(arr);
    return result;
}
