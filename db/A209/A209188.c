/*
 * A209188  Smallest prime factor of n^2 + n - 1.
 * https://oeis.org/A209188
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime factor of (n^2 + n - 1)
 * Range:     u8 n <= 15, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _smallest_prime_factor_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_smallest_prime_factor_seq.h"

/* Public sequence function. */
seq_t a(idx_t n)
{
    /* Indices below 2 are irrelevant; behaviour is undefined. */
    seq_t nn = (seq_t)n;
    seq_t N  = nn * (nn + (seq_t)1) - (seq_t)1;  /* n^2 + n - 1 */

    return _smallest_prime_factor_seq(N);
}
