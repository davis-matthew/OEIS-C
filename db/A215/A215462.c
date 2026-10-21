/*
 * A215462  Number of decompositions of 2n into ordered sums of two odd
 *          nonprimes.
 * https://oeis.org/A215462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered decompositions of 2*n into two odd
 *            nonprimes. Each pair (i, j) with i + j = 2*n, i and j odd, and
 *            neither i nor j prime (1 is considered nonprime)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t limit = 2 * n;           /* 2*n is even, stored in idx_t to avoid overflow */
    seq_t count = 0;

    /* iterate over all odd i from 1 to 2*n-1 */
    for (idx_t i = 1; i < limit; i += 2) {
        if (!is_prime(i) && !is_prime(limit - i)) {
            count++;
        }
    }
    return count;
}
