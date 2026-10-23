/*
 * A201307  Primes of the form k^3+4.
 * https://oeis.org/A201307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes of the form k^3+4; a(n) is the nth such prime (OEIS A201307) */
#include <stdbool.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        seq_t val = ((seq_t)k * (seq_t)k * (seq_t)k) + 4;
        if (is_prime_seq(val)) {
            ++count;
            if (count == n) {
                return val;
            }
        }
    }
}
