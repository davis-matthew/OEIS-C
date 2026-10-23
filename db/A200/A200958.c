/*
 * A200958  Primes of the form 8n^3-7.
 * https://oeis.org/A200958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime of the form 8*k^3 - 7
 * Range:     u8 none, u16 n <= 5, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_seq.h"

seq_t a(idx_t n) {
    idx_t found = 0;
    idx_t k = 1;
    seq_t val;
    while (found < n) {
        /* compute 8*k^3 - 7 in seq_t to avoid overflows */
        val = ((seq_t)8 * (seq_t)k * (seq_t)k * (seq_t)k) - (seq_t)7;
        if (is_prime_seq(val)) {
            ++found;
            if (found == n) return val;
        }
        ++k;
    }
    return 0; /* should never be reached */
}
