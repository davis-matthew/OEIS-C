/*
 * A144590  Number of ordered ways of writing 2n+1 = i + j, where i is a prime
 *          and j is of the form k*(k+1), k > 0.
 * https://oeis.org/A144590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of ordered ways of writing 2n+1 = i + j, where i
 *            is a prime and j is k*(k+1) with k > 0
 * Range:     u8 n <= 180, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* The odd number to be split. */
    idx_t target = (idx_t)2 * n + 1;
    seq_t count = 0;

    for (idx_t k = 1; ; ++k) {
        /* j = k(k+1) */
        idx_t j = k * (k + 1);
        if (j >= target) break;        /*  i would be <= 0 */

        idx_t i = target - j;          /* the prime candidate */
        if (is_prime_big((seq_t)i)) {
            ++count;                   /* found a valid i + j split */
        }
    }
    return count;
}
