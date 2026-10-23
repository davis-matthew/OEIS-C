/*
 * A153088  Numbers k such that 5*k - 1 is not prime.
 * https://oeis.org/A153088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: iterate k, count those where 5*k-1 is not prime */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t k = 1;
    while (1) {
        seq_t t = 5 * k - 1;
        if (!is_prime_big(t)) {
            count++;
            if (count == n) {
                return k;
            }
        }
        k++;
    }
}
