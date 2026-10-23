/*
 * A048911  Indices of square numbers which are also 9-gonal.
 * https://oeis.org/A048911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 30*a(n-2) - a(n-4) with initial values a(1)=1, a(2)=3,
 *            a(3)=33, a(4)=91
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)33;
    if (n == 4) return (seq_t)91;

    /* For n >= 5, use the linear recurrence.
       arr[0] = a(n-4), arr[1] = a(n-3), arr[2] = a(n-2), arr[3] = a(n-1) */
    seq_t arr[4] = {1, 3, 33, 91};

    for (idx_t i = 5; i <= n; ++i) {
        seq_t new_val = (seq_t)30 * arr[2] - arr[0];
        /* Shift the window forward: a(n-3) becomes a(n-4), etc. */
        arr[0] = arr[1];
        arr[1] = arr[2];
        arr[2] = arr[3];
        arr[3] = new_val;
    }

    return arr[3];   /* a(n) is in arr[3] after the loop */
}
