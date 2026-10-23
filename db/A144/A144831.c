/*
 * A144831  (n+1)^2 - (smallest prime > n^2).
 * https://oeis.org/A144831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^2 - next_prime(n^2) (with smallest prime > n^2)
 * Range:     u8 n <= 15, u16 n <= 1111, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* Handle indices below the offset safely */
    if (n == 0) {
        return 0;
    }

    /* Promote n to the arithmetic type of the sequence values */
    seq_t n_seq = (seq_t)n;

    /* Compute n^2 and (n+1)^2 in seq_t */
    seq_t n_sq   = n_seq * n_seq;
    seq_t np_sq  = (n_seq + 1) * (n_seq + 1);

    /* Find the smallest prime greater than n^2 */
    uint64_t next_prime_val = next_prime((uint64_t)n_sq);

    /* Subtract that prime from (n+1)^2 to obtain a(n) */
    seq_t result = np_sq - (seq_t)next_prime_val;

    return result;
}
