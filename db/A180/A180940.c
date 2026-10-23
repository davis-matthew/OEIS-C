/*
 * A180940  Primes of the form n^2 - 2999n + 2248541.
 * https://oeis.org/A180940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the n‑th prime of the form m² – 2999·m + 2248541
 * Range:     u8 none, u16 none, u32 none, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t m = 1;
    seq_t value;

    for (;; ++m) {
        value = m * m - (seq_t)2999 * m + (seq_t)2248541;
        if (is_prime_big(value)) {
            ++count;
            if (count == n)
                return value;
        }
    }
}
