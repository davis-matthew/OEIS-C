/*
 * A103570  Sum of the (primes > 5 modulo 13).
 * https://oeis.org/A103570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = sum_{i=1..n} (p_i mod 13) for primes p_i > 5
 * Range:     u8 n <= 39, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return 0;
    }
    seq_t sum = 0;
    idx_t count = 0;
    idx_t p = 7; /* the first prime greater than 5 */
    for (; count < n; ++p) {
        if (is_prime_u64(p)) {
            /* p % 13 fits into idx_t; cast to seq_t for the sum */
            sum += (seq_t)(p % 13);
            ++count;
        }
    }
    return sum;
}
