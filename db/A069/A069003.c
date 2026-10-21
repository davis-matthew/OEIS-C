/*
 * A069003  Smallest integer d such that n^2 + d^2 is a prime number.
 * https://oeis.org/A069003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest positive integer d such that n^2 + d^2 is prime
 * Range:     u8 n <= 15, u16 n <= 250, u32 n <= 427, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Returns the smallest d ≥ 1 so that n^2 + d^2 is prime.
   For n ≥ 1. */
seq_t a(idx_t n)
{
    /* n and d are taken as seq_t for correct arithmetic */
    seq_t nn = (seq_t)n * (seq_t)n;          /* precompute n^2 */
    for (seq_t d = 1;; ++d) {
        seq_t sum = nn + d * d;              /* n^2 + d^2 */
        if (is_prime_big(sum))
            return d;
    }
}
