/*
 * A104211  Integers n such that the sum of the digits of n is not prime.
 * https://oeis.org/A104211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 159, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The nth integer whose sum of digits is NOT prime.  Offset 1 (a(1)=1). */
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t m = 0;

    while (count < n) {
        ++m;
        /* Compute sum of digits of m (converted to seq_t). */
        int sum = (int)digit_sum((seq_t)m);
        /* If the sum is not prime, we count this number. */
        if (!is_prime_small(sum))
            ++count;
    }

    return (seq_t)m;
}
