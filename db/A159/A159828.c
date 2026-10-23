/*
 * A159828  a(n) is smallest number m > 0 such that m^2 + n^2 + 1 is prime.
 * https://oeis.org/A159828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the smallest m > 0 such that m^2 + n^2 + 1 is prime
 * Range:     u8 n <= 15, u16 n <= 328, u32 n <= 326, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Convert the index to seq_t before any arithmetic to avoid overflow
       on narrow integer types. */
    seq_t n_sq = (seq_t)n * ((seq_t)n);

    for (seq_t m = 1;; ++m) {
        seq_t val = m * m + n_sq + (seq_t)1;
        if (is_prime_big(val)) {
            return m;  /* found the smallest m giving a prime */
        }
    }
}
