/*
 * A095372  1+integers repeating "90" decimal digit pattern.
 * https://oeis.org/A095372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 101*a(n-1) - 100*a(n-2), a(0) = 1, a(1) = 91 This
 *            implements the recurrence for OEIS A095372. The computation is
 *            performed iteratively up to the requested index n, using only
 *            arithmetic on seq_t values as required by the build constraints
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, if needed */

seq_t a(idx_t n)
{
    /* Handle the two base cases explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)91;

    /* Initialize previous two terms of the sequence */
    seq_t a_nm2 = (seq_t)1;   /* a(n-2)  */
    seq_t a_nm1 = (seq_t)91;  /* a(n-1)  */
    seq_t a_n   = 0;          /* a(n)    */

    /* Iteratively compute up to the required index n */
    for (idx_t i = 2; i <= n; ++i)
    {
        a_n = (a_nm1 * (seq_t)101) - (a_nm2 * (seq_t)100);
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
