/*
 * A023230  Numbers k such that k and 8*k + 5 are both prime.
 * https://oeis.org/A023230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 159, u32 n <= 161, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th prime k such that k and 8*k+5 are both prime.
   The function scans integers starting at 2 and counts suitable primes
   until the n‑th one is found.  This works for any n >= 1 and does not
   rely on any global state. */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Count of found terms. */
    idx_t count = 0;
    /* Candidate k.  Use seq_t so that arithmetic stays in the right type. */
    seq_t k = 2;

    while (count < n) {
        if (is_prime_big(k)) {
            seq_t t = (seq_t)8 * k + 5;
            if (is_prime_big(t)) {
                ++count;
                if (count == n)
                    return k;
            }
        }
        ++k;
    }
    /* Should never reach here for valid n. */
    return 0;
}
