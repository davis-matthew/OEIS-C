/*
 * A179283  The smallest prime number greater than 2^n / 3.
 * https://oeis.org/A179283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 17, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest prime > 2^n/3, computed by iterating from the next integer past (2^n)/3 until a prime is found. */
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Compute 2^n as a seq_t */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow2 *= 2;              /* multiply by 2 n times */

    /* Threshold: floor(2^n / 3) */
    seq_t threshold = pow2 / 3;

    /* Candidate starting just above the threshold */
    seq_t candidate = threshold + 1;

    /* Find the smallest prime greater than the threshold */
    while (!is_prime_big(candidate))
        ++candidate;

    return candidate;
}
