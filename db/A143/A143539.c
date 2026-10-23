/*
 * A143539  Number of ways to express 2n-1 as p+2a^2; p prime, a > 0.
 * https://oeis.org/A143539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of 2*n-1 as p+2*a^2 with p
 *            prime and a>0
 * Range:     u8 n <= 291, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset starts at 1 */

    seq_t m = (seq_t)n * 2 - 1;    /* odd number 2n-1 */
    seq_t count = 0;

    for (idx_t a = 1; ; ++a) {
        seq_t a2      = (seq_t)a * a;     /* a^2   */
        seq_t two_a2  = a2 << 1;          /* 2a^2  */

        if (two_a2 > m) break;           /* 2a^2 > 2n-1 → done */

        seq_t p = m - two_a2;             /* candidate prime */

        if (p >= (seq_t)2 && is_prime_big(p))
            ++count;
    }

    return count;
}
