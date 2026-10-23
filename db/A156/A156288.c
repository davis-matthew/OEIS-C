/*
 * A156288  Numbers n such that 4*n-5 is not a prime number.
 * https://oeis.org/A156288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th positive integer m such that (4*m - 5) is not a prime. */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;
    idx_t count = 0;
    idx_t k = 5;  /* the smallest n with 4*n-5 composite and >1 */
    while (1) {
        seq_t val = (seq_t)k * ((seq_t)4) - ((seq_t)5);
        if (!is_prime_big(val)) {
            ++count;
            if (count == n) return (seq_t)k;
        }
        ++k;
    }
}
