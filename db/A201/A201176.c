/*
 * A201176  Primes of the form 5n^3+7.
 * https://oeis.org/A201176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime of the form 5k^3 + 7 (search)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return 0; /* undefined for n < 1, but return 0 safely */
    }
    idx_t cnt = 0;
    for (idx_t k = 0; ; ++k) {
        seq_t kk = (seq_t)k;
        seq_t val = ((seq_t)5 * kk * kk * kk) + (seq_t)7;
        if (is_prime_seq(val)) {
            ++cnt;
            if (cnt == n) {
                return val;
            }
        }
    }
}
