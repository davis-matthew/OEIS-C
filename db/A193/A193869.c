/*
 * A193869  Smallest product of n distinct primes of the form n*k + 1.
 * https://oeis.org/A193869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   smallest product of n distinct primes of the form n*k + 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t product = 1;
    idx_t count   = 0;
    idx_t m       = 1;
    seq_t sn      = (seq_t)n;

    while (count < n) {
        seq_t p = sn * (seq_t)m + 1;
        if (is_prime_big(p)) {
            product *= p;
            ++count;
        }
        ++m;
    }
    return product;
}
