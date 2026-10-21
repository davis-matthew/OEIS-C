/*
 * A062501  Number of distinct prime divisors of the nonprimes (including 1).
 * https://oeis.org/A062501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001221(A018252(n)) (number of distinct prime divisors of
 *            the n‑th nonprime including 1)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* n==0 is outside the official sequence */
    /* Find the n-th nonprime (including 1). */
    idx_t count = 0;
    idx_t x = 1;
    for (;; ++x) {
        if (x == 1 || !is_prime(x)) {
            ++count;
            if (count == n) break;
        }
    }
    /* Count distinct prime divisors of x. */
    uint64_t m = x;
    uint64_t distinct = 0;
    if (m > 1) {
        for (uint64_t p = 2; p <= m / p; ++p) {
            if (m % p == 0) {
                ++distinct;
                while (m % p == 0) m /= p;
            }
        }
        if (m > 1) ++distinct;   /* remaining prime factor */
    }
    return (seq_t)distinct;
}
