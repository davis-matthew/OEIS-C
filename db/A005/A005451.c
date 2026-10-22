/*
 * A005451  a(n) = 1 if n is a prime number, otherwise a(n) = n.
 * https://oeis.org/A005451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 1 if n is prime, else n
 * Range:     u8 n <= 255, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_local.h"

seq_t a(idx_t n) {
    if (is_prime_local(n)) {
        return (seq_t)1;
    } else {
        return (seq_t)n;
    }
}
