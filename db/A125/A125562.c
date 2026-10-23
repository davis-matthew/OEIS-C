/*
 * A125562  a(n) = denominator of (2n + 1)!/3^n.
 * https://oeis.org/A125562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 3^max(0, n - Σ_{k≥1} floor((2n+1)/3^k))
 * Range:     u8 n <= 727, u16 n <= 6464, u32 n <= 6464, u64 n <= 6464,
 *            u128 n <= 6464, big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The denominator of (2n+1)!/3^n is a power of 3 given by the above expression. */
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* exponent of 3 in (2n+1)! */
    idx_t m = (idx_t)2 * n + 1;
    idx_t e3 = 0;
    while (m > 0) {
        m /= 3;
        e3 += m;
    }

    /* if e3 >= n the fraction is an integer, otherwise the denominator is 3^(n-e3) */
    idx_t diff = (n > e3) ? (n - e3) : 0;
    return pow3(diff);
}
