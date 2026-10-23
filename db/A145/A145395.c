/*
 * A145395  Complement of the primes of form 4k+3 (A002145).
 * https://oeis.org/A145395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = nth nonnegative integer that is NOT a prime of the form 4k+3 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    seq_t x = 0;
    idx_t count = 0;

    while (1) {
        bool skip = false;
        if (x >= (seq_t)2) {
            if (is_prime_big(x) && ((x & (seq_t)3) == (seq_t)3))
                skip = true;
        }
        if (!skip) {
            count++;
            if (count == n)
                return x;
        }
        x++; /* next integer */
    }
}
