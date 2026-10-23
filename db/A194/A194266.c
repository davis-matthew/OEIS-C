/*
 * A194266  Smallest sum of two distinct primes of the form n*k+1.
 * https://oeis.org/A194266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest sum of two distinct primes of the form n*k+1 *
 *            (OEIS sequence A194266)
 * Range:     u8 n <= 18, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* guard for offset, though a(0) is unused */

    idx_t k     = 1;     /* loop counter for k in n*k+1 */
    idx_t p1    = 0;     /* first prime found */
    idx_t p2    = 0;     /* second distinct prime */

    /* Find the first prime of the form n*k+1. */
    for (;; ++k) {
        idx_t cand = n * k + 1;   /* always fits in idx_t for the problem size */
        if (is_prime(cand)) { p1 = cand; break; }
    }

    /* Find the second distinct prime of the same form. */
    ++k; /* start searching after the first prime */
    for (;; ++k) {
        idx_t cand = n * k + 1;
        if (is_prime(cand) && cand != p1) { p2 = cand; break; }
    }

    /* Sum and return the result, converting to seq_t. */
    return (seq_t)p1 + (seq_t)p2;
}
