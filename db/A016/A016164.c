/*
 * A016164  Expansion of 1/((1-5*x)*(1-10*x)).
 * https://oeis.org/A016164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*10^n - 5^n, a(0)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   pow10, power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/power5.h"

seq_t a(idx_t n)
{
    /* compute 10^n and 5^n */
    seq_t p10 = pow10(n);
    seq_t p5  = power5(n);

    /* a(n) = 2*10^n - 5^n */
    seq_t res = p10 * 2;
    res -= p5;
    return res;
}
