/*
 * A117481  a(n) = n*(n-1)*(n-2)*(n-3)*...*(n-k) such that (n-k) is the
 *          largest prime smaller than n.
 * https://oeis.org/A117481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Π_{k=prevprime(n)}^{n} k with a(1)=1, a(2)=2 (where
 *            prevprime(n) is the largest prime less than n)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 28, u64 n <= 121, u128 n <= 536,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* handle the two special cases */
    if (n <= 1) return (seq_t)1;          /* a(1) = 1                                 */
    if (n == 2) return (seq_t)2;          /* a(2) = 2                                 */

    /* find the largest prime smaller than n */
    idx_t p = n - 1;
    while (!is_prime(p)) {
        /* n is at least 3, so this loop terminates before p becomes 0   */
        p--;
    }

    /* multiply all integers from p up to n */
    seq_t prod = (seq_t)1;
    for (idx_t x = p; x <= n; ++x) {
        prod = prod * (seq_t)x;
    }
    return prod;
}
