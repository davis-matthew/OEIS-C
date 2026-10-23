/*
 * A107231  a(n) = C(n+2,2)*C(n,floor(n/2)).
 * https://oeis.org/A107231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = C(n+2,2) * C(n, floor(n/2))
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 26, u64 n <= 56, u128 n <= 118,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* C(n+2,2) = (n+2)(n+1)/2 */
    seq_t first = binomial(n + 2, 2);
    /* C(n, floor(n/2)) */
    seq_t second = binomial(n, n / 2);
    return first * second;
}
