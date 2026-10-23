/*
 * A187273  a(n) = (n/4)*3^(n/2)*((1+sqrt(3))^2+(-1)^n*(1-sqrt(3))^2).
 * https://oeis.org/A187273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(n/2)*3^(n/2) for even n a(n) = n * 3^((n+1)/2) for odd
 *            n Derived from a(2k) = 4k*3^k and a(2k+1) = (2k+1)*3^(k+1)
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 33, u64 n <= 71, u128 n <= 151,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {                    /* even n = 2*k */
        idx_t k = n / 2;                   /* k = n/2 */
        seq_t val = pow3(k);               /* 3^k */
        val *= (seq_t)(4 * k);             /* 4*k * 3^k */
        return val;
    } else {                               /* odd n = 2*k + 1 */
        idx_t k = (n + 1) / 2;             /* k = (n+1)/2 */
        seq_t val = pow3(k);               /* 3^((n+1)/2) */
        val *= (seq_t)n;                   /* n * 3^((n+1)/2) */
        return val;
    }
}
