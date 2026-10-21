/*
 * A234041  a(n) = binomial(n+2,2)*gcd(n,3)/3, n >= 0.
 * https://oeis.org/A234041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+2,2)*gcd(n,3)/3
 * Range:     u8 n <= 14, u16 n <= 280, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t n1 = (seq_t)n + 1;
    seq_t n2 = (seq_t)n + 2;
    seq_t prod = n1 * n2;               /* (n+1)(n+2) */

    if (n % 3 == 0)                     /* gcd(n,3) = 3 */
        return prod / 2;                /* divide by 2  */
    else                                /* gcd(n,3) = 1 */
        return prod / 6;                /* divide by 6  */
}
