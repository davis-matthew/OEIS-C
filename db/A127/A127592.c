/*
 * A127592  Primes of the form 64k+21.
 * https://oeis.org/A127592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 213, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Computes the nth prime of the form 64k+21 (OEIS A127592). */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* not used, but guard edge case */
    idx_t count = 0;
    idx_t k = 0;
    while (1) {
        idx_t cand = 21 + 64 * k;
        if (is_prime(cand)) {
            ++count;
            if (count == n) {
                return (seq_t)cand;
            }
        }
        ++k;
    }
}
