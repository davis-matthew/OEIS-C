/*
 * A163114  a(n) = 5*a(n-2) for n > 2; a(1) = 3, a(2) = 5.
 * https://oeis.org/A163114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-2), a(1)=3, a(2)=5
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 54, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t in case idx_t is that type */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)5;

    /* compute 5^floor((n-1)/2) by binary exponentiation */
    idx_t exp = (n - 1) / 2;               /* floor((n-1)/2) */
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)5;

    while (exp)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }

    /* final factor: 3 for odd n, 5 for even n */
    if (n & 1)   /* odd */
        result *= (seq_t)3;
    else         /* even */
        result *= (seq_t)5;

    return result;
}
