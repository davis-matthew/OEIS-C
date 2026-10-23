/*
 * A111175  Numbers k such that 30*k + 1 is prime.
 * https://oeis.org/A111175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k such that 30*k + 1 is prime. The
 *            function searches sequentially and tests primality using the
 *            provided is_prime helper
 * Range:     u8 n <= 116, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Ensure n >= 1; behaviour for n==0 is undefined for this sequence. */
    idx_t count = 0;
    idx_t k = 0;
    while (true) {
        k++;
        uint64_t p = 30ULL * (uint64_t)k + 1ULL;
        if (is_prime(p)) {
            count++;
            if (count == n) {
                return (seq_t)k;
            }
        }
    }
}
