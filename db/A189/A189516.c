/*
 * A189516  a(n) = n + [nr/s] + [nt/s]; r=1, s=Pi/2, t=2/Pi.
 * https://oeis.org/A189516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊2n/π⌋ + ⌊(4/π²)n⌋
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* use long double constants for accurate floor computation */
    const long double pi          = acosl(-1.0L);
    const long double two_over_pi = 2.0L / pi;
    const long double four_over_pi2 = 4.0L / (pi * pi);

    /* compute each floor term as an integer type that fits in idx_t */
    idx_t t1 = (idx_t)floorl(n * two_over_pi);
    idx_t t2 = (idx_t)floorl(n * four_over_pi2);

    /* convert everything to seq_t before summing */
    return (seq_t)n + (seq_t)t1 + (seq_t)t2;
}
