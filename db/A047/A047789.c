/*
 * A047789  Denominators of Glaisher's I-numbers.
 * https://oeis.org/A047789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(0)=2, a(3*n)=1, a(3*n+2)=1, a(3*n+1)=3*a(n)
 * Range:     u8 n <= 363, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return 2;
    if (n == 1) return 3;
    if (n == 2) return 1;

    seq_t res = 1;

    /* Reduce n while it is of the form 3*k+1, accumulating powers of 3 */
    while (n >= 3) {
        if (n % 3 == 0 || n % 3 == 2)
            break;               /* a(3*k) = a(3*k+2) = 1 */
        res *= 3;                 /* a(3*k+1) = 3*a(k)   */
        n /= 3;
    }

    /* Handle the remaining base value */
    if (n == 0)         res *= 2;   /* a(0) = 2 */
    else if (n == 1)    res *= 3;   /* a(1) ≠ 3*a(0) */
    /* n==2 or n%3==0/2 already results in multiplication by 1 */

    return res;
}
