/*
 * A225788  a(n) = floor(72*n^(1/2)*(log(n))^(3/2)) for n >= 1, a(0) = 0.
 * https://oeis.org/A225788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(72*n^(1/2)*(log(n))^(3/2)) for n >= 1, a(0) = 0
 * Range:     u8 n <= 4, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Use long double for maximum precision */
    long double dn = (long double)n;
    long double val = 72.0L * sqrtl(dn) * powl(logl(dn), 1.5L);

    /* add tiny epsilon to guard against round‑off that could push the
       floor one step down */
    unsigned long long res = (unsigned long long)(floorl(val + 1e-12L));

    return (seq_t)res;
}
