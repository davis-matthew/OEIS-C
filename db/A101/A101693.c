/*
 * A101693  A modular binomial sum transform of 2^n.
 * https://oeis.org/A101693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} (binomial(2n-2, k) mod 2) * 2^k
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* a(0) = 1 */

    idx_t m = n - 1;               /* n-1 */
    idx_t half = n >> 1;           /* floor(n/2) */

    seq_t sum = 0;
    for (idx_t t = m; ; t = (t - 1) & m) {
        /* include only submasks t with 2*t <= n  i.e., t <= half */
        if (t <= half) {
            /* add 2^(2*t) = 4^t */
            sum += pow2((idx_t)(2 * t));
        }
        if (t == 0) break;         /* last submask processed */
    }
    return sum;
}
