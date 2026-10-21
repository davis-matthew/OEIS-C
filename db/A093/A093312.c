/*
 * A093312  Least k such that (2n + 2k) + 1 and (2n*2k) + 1 are both primes.
 * https://oeis.org/A093312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k >= 1 such that (2(n+k)+1) and (4nk+1) are both
 *            prime
 * Range:     u8 n <= 10, u16 n <= 1135, u32 n <= 541, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Sequence is defined for n >= 1; return 0 for n == 0 as a safe fallback. */
    if (n == 0) return 0;

    idx_t k = 1;
    while (true) {
        seq_t v1 = 2 * ((seq_t)n + (seq_t)k) + 1;          /* 2(n + k) + 1 */
        seq_t v2 = 4 * ((seq_t)n * (seq_t)k) + 1;          /* 4 n k + 1 */
        if (is_prime_big(v1) && is_prime_big(v2)) {
            return (seq_t)k;
        }
        ++k;
    }
}
