/*
 * A187409  n^2 + nextprime(n^2).
 * https://oeis.org/A187409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 + nextprime(n^2)
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* n^2 fits into 64 bits for the intended use range */
    uint64_t sq = (uint64_t)n * (uint64_t)n;
    uint64_t p  = next_prime(sq);          /* smallest prime > n^2 */
    return (seq_t)n * (seq_t)n + (seq_t)p;
}
