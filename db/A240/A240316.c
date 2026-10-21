/*
 * A240316  Number of n X 2 0..3 arrays with no element equal to two plus the
 *          sum of elements to its left or two plus the sum of elements above
 *          it or zero plus the sum of the elements diagonally to its
 *          northwest, modulo 4.
 * https://oeis.org/A240316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence: a(n) = 12*a(n-2) - 24*a(n-4) + 31*a(n-6) - 16*a(n-8);
   Initial values (offset 1): a(1) = 2, a(2) = 4, a(3) = 18, a(4) = 44,
   a(5) = 184, a(6) = 444, a(7) = 1826, a(8) = 4388. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle invalid index (offset is 1). */
    if (n == 0)
        return (seq_t)0;

    /* Base values for n = 1 .. 8. */
    static const seq_t base[8] = {2, 4, 18, 44, 184, 444, 1826, 4388};

    /* For n <= 8 return the cached base value. */
    if (n <= 8)
        return base[n - 1];

    /* Sliding window of the last 8 terms: arr[0] = a(k-8), ..., arr[7] = a(k-1). */
    seq_t arr[8];
    for (idx_t i = 0; i < 8; ++i)
        arr[i] = base[i];

    seq_t val;
    for (idx_t k = 9; k <= n; ++k) {
        /* a(k) = 12*a(k-2) - 24*a(k-4) + 31*a(k-6) - 16*a(k-8) */
        val = 12 * arr[6]          /* a(k-2) */
              - 24 * arr[4]        /* a(k-4) */
              + 31 * arr[2]        /* a(k-6) */
              - 16 * arr[0];       /* a(k-8) */

        /* Shift the window left: discard a(k-8), make room for a(k). */
        for (idx_t i = 0; i < 7; ++i)
            arr[i] = arr[i + 1];
        arr[7] = val;             /* now arr[7] holds a(k) */
    }

    return arr[7]; /* a(n) */
}
