/*
 * A225569  Decimal expansion of Sum_{n>=0} 1/10^(3^n), a transcendental
 *          number.
 * https://oeis.org/A225569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   a(n) = 1 if (n+1) is a power of 3, otherwise 0
 * Range:     u8 n <= 8230, u16 n <= 8230, u32 n <= 8230, u64 n <= 8230,
 *            u128 n <= 8230, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute k = n + 1 ;  use idx_t to avoid overflow if n is ULLONG_MAX */
    idx_t k = n + 1;

    /* divide by 3 while divisible */
    while (k % 3 == 0)
        k /= 3;

    /* if the remaining factor is 1, then n+1 was a power of 3 */
    seq_t result = (k == 1) ? (seq_t)1 : (seq_t)0;
    return result;
}
