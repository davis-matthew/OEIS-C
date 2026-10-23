/*
 * A127591  Numbers k such that 64k+21 is prime.
 * https://oeis.org/A127591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that 64*k+21 is prime. Incrementally search for k
 * Range:     u8 n <= 1, u16 n <= 237, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    idx_t k = 0;
    idx_t cnt = 0;
    while (cnt < n) {
        seq_t val = (seq_t)k * (seq_t)64 + (seq_t)21;
        if (is_prime_small(val)) {
            cnt++;
            if (cnt == n)
                return (seq_t)k;
        }
        k++;
    }
    /* Should never be reached */
    return (seq_t)0;
}
