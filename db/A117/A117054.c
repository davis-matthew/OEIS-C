/*
 * A117054  Number of ordered ways of writing n = i + j, where i is a prime
 *          and j is of the form k*(k+1), k > 0.
 * https://oeis.org/A117054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = # { (p,k) : p prime, k>0, n = p + (k*(k+1)-1) }
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    seq_t count = 0;
    idx_t j = 1;          /* j = k*(k+1)-1 for k = 1 */
    idx_t k = 1;
    while (1) {
        if (j > n) break;                         /* no further j can be used */
        idx_t p = n - j;
        if (p >= 2 && is_prime(p)) ++count;

        /* compute increment for next j: Δ = 2k+2 */
        idx_t inc = (k << 1) + 2;
        if (j > UINT64_MAX - inc) break;          /* would overflow j */
        j += inc;
        ++k;
    }
    return count;
}
