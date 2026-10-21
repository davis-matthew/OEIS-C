/*
 * A083658  a(n) = a(n-1) + a(n-2) + gcd(a(n-1), a(n-2)) for n > 1; a(0)=1,
 *          a(1)=1.
 * https://oeis.org/A083658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n)=3^n, a(2n+1)=5*3^(n-1) for n>0; a(0)=1, a(1)=1
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 40, u64 n <= 80, u128 n <= 161,
 *            big any n
 * Helpers:   pow_of_3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_of_3.h"

seq_t a(idx_t n)
{
    if (n == 0 || n == 1) return 1;
    if ((n & 1) == 0)   /* even n : n = 2k */
    {
        idx_t k = n >> 1;
        return pow_of_3(k);          /* 3^k */
    }
    else                  /* odd n : n = 2k+1 , k>0 */
    {
        idx_t k = n >> 1;            /* floor((n-1)/2) */
        seq_t v = pow_of_3(k - 1);   /* 3^(k-1) */
        v = v * 5;                   /* multiply by 5 */
        return v;
    }
}
