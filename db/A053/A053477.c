/*
 * A053477  Sum of iterates of divisor number function A000005.
 * https://oeis.org/A053477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + d(n) + d(d(n)) + ... until the fixed point 1 for n =
 *            1 or 2 for n > 1, where d(k) is the number of divisors of k.
 *            This implementation uses a small trial‑division routine to
 *            compute d(k)
 * Range:     u8 n <= 233, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   local_divisor_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/local_divisor_count.h"

/* Public function required by the problem. */
seq_t a(idx_t n)
{
    /* a(1) = 1 */
    if (n == 1)
        return (seq_t)1;

    uint64_t cur = n;
    seq_t sum = (seq_t)cur;   /* start with n itself */

    /* Iterate divisor counts until the fixed point 2 is reached. */
    while (cur != 2) {
        cur = local_divisor_count(cur);
        sum += (seq_t)cur;
    }

    return sum;
}
