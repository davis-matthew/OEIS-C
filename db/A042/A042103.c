/*
 * A042103  Denominators of continued fraction convergents to sqrt(575).
 * https://oeis.org/A042103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 48*a(n-2) - a(n-4) for n>3 Initial values: a(0)=1,
 *            a(1)=1, a(2)=47, a(3)=48
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t, if seq_t is a typedef to an integer type */

/* Assume seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 47;
    if (n == 3) return 48;

    /* Use a rolling array of size 4 to store the last four terms. */
    seq_t arr[4] = {1, 1, 47, 48};

    for (idx_t i = 4; i <= n; ++i)
    {
        /* recurrence: a(i) = 48*a(i-2) - a(i-4) */
        arr[i & 3] = 48 * arr[(i - 2) & 3] - arr[(i - 4) & 3];
    }

    return arr[n & 3];
}
