/*
 * A076598  Sum of squares of divisors d of n such that d or n/d is odd.
 * https://oeis.org/A076598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(n) = sigma_2(n) - 4 * sigma_2(n/4) if n mod 4 == 0 otherwise
 *            a(n) = sigma_2(n)
 * Range:     u8 n <= 14, u16 n <= 221, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mul_small, sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_k.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for n=0, return 0 for safety */
        return 0;

    seq_t sum = sigma_k(n, 2);
    if ((n & 3U) == 0)   /* n is divisible by 4 */
    {
        seq_t sum4 = sigma_k(n >> 2, 2);   /* n/4 */
        seq_t sub = mul_small(sum4, 4);
        sum -= sub;
    }
    return sum;
}
