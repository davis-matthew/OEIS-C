/*
 * A201964  Primes of the form 10n^2 - 9.
 * https://oeis.org/A201964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 29, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Prime of the form 10k²-9, a(n) = the n-th such prime (k starts at 2) */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_uint64.h"

seq_t a(idx_t n)
{
    /* n is the index (1‑based). We scan k=2,3,… until we find n primes of the
       form 10k²-9. */
    uint64_t k = 2;          /* candidate k */
    uint64_t found = 0;      /* how many primes so far */

    while (found < n) {
        /* Compute 10k²-9 in seq_t to keep the arithmetic type consistent. */
        seq_t val = (seq_t)10 * (seq_t)k * (seq_t)k - (seq_t)9;

        /* Test for primality.  The values fit in uint64_t for n up to the
           practical limits of the sequence (here ≈10⁴). */
        if (is_prime_uint64((uint64_t)val)) {
            ++found;
            if (found == n)
                return val;
        }
        ++k;
    }
    /* Unreachable, but keeps the compiler happy. */
    return 0;
}
