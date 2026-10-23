/*
 * A046915  Sum of divisors of 10^n.
 * https://oeis.org/A046915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1/4*(2^(n+1)-1)*(5^(n+1)-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   pow2, power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/power5.h"

seq_t a(idx_t n)
{
    seq_t two = pow2(n + 1);   /* 2^(n+1) */
    two -= 1;                  /* 2^(n+1) - 1 */

    seq_t five = power5(n + 1); /* 5^(n+1) */
    five -= 1;                  /* 5^(n+1) - 1 */

    seq_t prod = two * five;    /* (2^(n+1)-1)*(5^(n+1)-1) */

    return prod >> 2;           /* divide by 4 (shifts right 2) */
}
