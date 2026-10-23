/*
 * A129292  Number of divisors of n^4 - 1 that are not greater than n.
 * https://oeis.org/A129292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{d : 1 ≤ d ≤ n and (n^4−1) mod d = 0}
 * Range:     u8 n <= 2392, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t count = 0;               /* result to return */

    for (idx_t d = 1; d <= n; ++d) {
        /* compute n^4 modulo d, avoiding overflow */
        unsigned long long nd   = (unsigned long long)n % d;
        unsigned long long nd2  = (nd * nd) % d;   /* n^2 mod d */
        unsigned long long nd4  = (nd2 * nd2) % d; /* n^4 mod d */

        /* (n^4−1) ≡ 0 (mod d)  <=>  n^4 mod d ≡ 1 (mod d)  */
        if ((nd4 + d - 1) % d == 0) {
            ++count;
        }
    }

    return count;
}
