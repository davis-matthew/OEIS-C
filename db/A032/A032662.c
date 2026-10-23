/*
 * A032662  a(n) is the least k such that k concatenated with k + n is prime.
 * https://oeis.org/A032662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = least k ≥ 1 such that concat_parts(k, k + n) is prime
 * Range:     u8 n <= 9, u16 n <= 83, u32 n <= 83, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   concat_parts, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the least k for which the decimal concatenation of k and k+n is prime. */
#include <stdbool.h>
#include "../utils/concat_parts.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    for (idx_t k = 1; ; ++k) {
        seq_t c = concat_parts(k, k + n); /* concatenate k and k+n */
        if (is_prime_big(c))               /* test primality of the concat */
            return (seq_t)k;              /* found the least such k */
    }
}
