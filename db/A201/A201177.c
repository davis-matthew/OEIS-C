/*
 * A201177  Primes of the form 5n^3+8.
 * https://oeis.org/A201177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes of the form 5*n^3+8
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    idx_t found = 0;
    for (idx_t k = 1; ; ++k) {
        /* compute 5*k^3+8 in seq_t arithmetic */
        seq_t kk  = (seq_t)k;
        seq_t val = 5 * kk * kk * kk + 8;
        if (is_prime_seq(val)) {
            ++found;
            if (found == n)
                return val;
        }
    }
}
