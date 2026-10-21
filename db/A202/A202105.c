/*
 * A202105  Numbers k such that 90*k + 43 is prime.
 * https://oeis.org/A202105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that 90*k + 43 is prime. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* Find the n-th non‑negative integer k for which 90*k + 43 is prime. */
    /* The sequence is indexed starting from 1 (offset 1). */
    idx_t count = 0;          /* Number of primes found so far. */
    idx_t k = 0;              /* Current candidate k. */

    for (;;) {
        /* Candidate prime is 90*k + 43.  Use idx_t for the intermediate
           calculation to avoid overflow for normal usage. */
        idx_t cand = 90 * k + 43;

        if (is_prime_big((seq_t)cand)) {
            ++count;
            if (count == n)
                return (seq_t)k;   /* Return k itself. */
        }
        ++k;
    }
}
