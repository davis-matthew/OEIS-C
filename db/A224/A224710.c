/*
 * A224710  The number of unordered partitions {a,b} of 2n-1 such that a and b
 *          are composite.
 * https://oeis.org/A224710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - 2 - primepi(2n-4) (for n>1), a(1)=0
 * Range:     u8 n <= 394, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/prime_pi.h"

seq_t a(idx_t n) {
    if (n <= 1) return 0;

    /* Compute the prime counting function up to 2n-4 */
    idx_t limit = (idx_t)2 * n - 4;
    seq_t pi_val = prime_pi(limit);

    /* a(n) = n - 2 - pi(2n-4) */
    seq_t result = (seq_t)n - 2 - pi_val;
    return result;
}
