/*
 * A082418  Numbers k such that P(k) > P(k+1) < P(k+2), where P(n) = largest
 *          prime factor of n (A006530).
 * https://oeis.org/A082418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the n-th integer k such that
 *          P(k) > P(k+1) < P(k+2)   (P(n) = largest prime factor of n).
 * Computation: brute‑force search of integers until the nth satisfying
 *              integer is found.  This meets the required time bounds for
 *              n up to 10000.
 */

#include <stdint.h>
#include "../utils/largest_prime_factor.h"

seq_t a(idx_t n)
{
    idx_t k = 1;          /* current candidate k */
    idx_t found = 0;      /* how many satisfying terms seen so far */

    while (found < n) {
        /* Compute largest prime factors of k, k+1, k+2 */
        uint64_t pk  = largest_prime_factor((uint64_t)k);
        uint64_t pk1 = largest_prime_factor((uint64_t)(k + 1));
        uint64_t pk2 = largest_prime_factor((uint64_t)(k + 2));

        /* Check the "valley" condition */
        if (pk > pk1 && pk1 < pk2) {
            ++found;
            if (found == n) {
                return (seq_t)k;      /* k fits into seq_t by construction */
            }
        }
        ++k;
    }

    /* Should never reach here for valid n >= 1 */
    return 0;
}
