/*
 * A166260  a(n) = A089026(n) - 1.
 * https://oeis.org/A166260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-1 if n is prime, otherwise 0
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   _is_prime_lj
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/_is_prime_lj.h"

seq_t a(idx_t n) {
    if (_is_prime_lj(n))
        return (seq_t)(n - 1);
    else
        return (seq_t)0;
}
