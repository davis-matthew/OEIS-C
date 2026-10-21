/*
 * A077245  Bisection (even part) of Chebyshev sequence with Diophantine
 *          property.
 * https://oeis.org/A077245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n)=8*a(n-1)-a(n-2), a(0)=1, a(1)=10
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/*
 * Compute the OEIS sequence A077245.
 * Works for any seq_t and idx_t defined by the build environment.
 */
seq_t a(idx_t n)
{
    /* Handle the two starting values explicitly. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)10;

    /* Iteratively apply the linear recurrence. */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)10;  /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t a2 = (seq_t)8 * a1 - a0;  /* a(i) = 8*a(i-1) - a(i-2) */
        a0 = a1;
        a1 = a2;
    }
    return a1;
}
