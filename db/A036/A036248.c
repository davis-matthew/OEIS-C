/*
 * A036248  Denominator of fraction equal to the continued fraction [ 2, 3, 5,
 *          ..., prime(n) ].
 * https://oeis.org/A036248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, easy, nonn
 * Formula:   a(n) = A000040(n) * a(n-1) + a(n-2); with a(1) = 1, a(2) = 3
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"   /* provides:   uint32_t get_prime(idx_t k); */

seq_t a(idx_t n)
{
    if (n < 1)                /* undefined for n < 1, return 0 */
        return (seq_t)0;

    if (n == 1)               /* a(1) = 1 */
        return (seq_t)1;

    if (n == 2)               /* a(2) = 3 */
        return (seq_t)3;

    /* Compute iteratively: a(i) = p(i) * a(i-1) + a(i-2) */
    seq_t prev2 = (seq_t)1;   /* a(1) */
    seq_t prev1 = (seq_t)3;   /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        uint32_t p = get_prime(i);          /* prime(i) */
        seq_t cur = (seq_t)p * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
