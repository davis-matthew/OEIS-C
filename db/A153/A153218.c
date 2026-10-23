/*
 * A153218  Numbers k such that 6k + 7 is prime.
 * https://oeis.org/A153218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th nonnegative integer k such that 6k + 7 is prime
 * Range:     u8 n <= 116, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation simply scans increasing k and counts the desired
   numbers until the n-th one is found.  The primality test used is
   is_prime(idx_t), which is available in the utils directory. */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;

    while (true) {
        if (is_prime(6 * k + 7)) {
            ++count;
            if (count == n)
                return (seq_t)k;   /* return the k that yields the n-th prime */
        }
        ++k;
    }
}
