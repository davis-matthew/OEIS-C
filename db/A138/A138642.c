/*
 * A138642  Primes of form 19*n-3.
 * https://oeis.org/A138642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   the nth prime of the form 19*k-3, computed by scanning
 *            candidates 19*k-3 for k>=1 and selecting the nth prime
 * Range:     u8 n <= 2, u16 n <= 275, u32 n <= 321, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* In the OEIS offset a(1) = 73, so n>=1. */
    idx_t cnt = 0;
    idx_t k   = 1;

    while (1) {
        seq_t cand = (seq_t)19 * (seq_t)k - (seq_t)3;
        if (is_prime_big(cand)) {
            ++cnt;
            if (cnt == n)
                return cand;
        }
        ++k;
    }
}
