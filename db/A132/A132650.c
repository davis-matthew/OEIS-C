/*
 * A132650  a(n) = d(n)^d(n), where d(n) = A000005(n).
 * https://oeis.org/A132650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = d(n)^d(n), where d(n) = number of positive divisors of n
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 47, u64 n <= 119, u128 n <= 719,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute d(n) by iterating over divisors up to sqrt(n). */
    idx_t cnt = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* If i is the square root, count only once. */
            if (i == n / i) {
                ++cnt;
            } else {
                cnt += 2;
            }
        }
    }
    /* Return cnt^cnt using the provided big exponentiation routine. */
    return big_pow((seq_t)cnt, (seq_t)cnt);
}
