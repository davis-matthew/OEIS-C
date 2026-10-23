/*
 * A046109  Number of lattice points (x,y) on the circumference of a circle of
 *          radius n with center at (0,0).
 * https://oeis.org/A046109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 4 * ∏{p^e‖n , p ≡ 1 (mod 4)} (2*e+1) with a(0)=1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t prod = (seq_t)1;
    idx_t tmp   = n;
    idx_t i     = 2;

    while (i * i <= tmp) {
        if (tmp % i == 0) {
            idx_t e = 0;
            while (tmp % i == 0) {
                tmp /= i;
                e++;
            }
            if (i % 4 == 1) {
                prod *= (seq_t)(2 * e + 1);
            }
        }
        i++;
    }

    if (tmp > 1) {           /* remaining prime factor */
        if (tmp % 4 == 1) {
            prod *= (seq_t)3; /* 2*1+1 */
        }
    }

    return prod * (seq_t)4;
}
