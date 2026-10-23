/*
 * A010914  Pisot sequence E(5,17), a(n) = floor(a(n-1)^2 / a(n-2) + 1/2).
 * https://oeis.org/A010914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2), a(0)=5, a(1)=17
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 34, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial terms */
    const seq_t a0_initial = 5;
    const seq_t a1_initial = 17;

    if (n == 0) return a0_initial;
    if (n == 1) return a1_initial;

    /* Iteratively apply the linear recurrence */
    seq_t a0 = a0_initial;
    seq_t a1 = a1_initial;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t a2 = 4 * a1 - 2 * a0;
        a0 = a1;
        a1 = a2;
    }
    return a1;
}
