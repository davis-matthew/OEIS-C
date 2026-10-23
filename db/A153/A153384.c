/*
 * A153384  Numbers k such that 24*k+1 is not prime.
 * https://oeis.org/A153384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A153384: numbers k such that 24*k+1 is NOT prime.
   The function enumerates k starting from 0 and returns the n‑th value
   for which 24*k+1 fails to be prime.  Primality is checked with
   is_prime_big, which works for all supported seq_t sizes. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    idx_t found = 0;   /* how many suitable k's we have found */
    idx_t k = 0;       /* current candidate k */

    for (;; ++k) {
        /* Compute 24*k+1 using an unsigned 64‑bit integer to avoid
           overflow on the multiplication, then cast to seq_t. */
        uint64_t m64 = 24ULL * (uint64_t)k + 1;
        seq_t m = (seq_t)m64;

        /* 1 is composite; otherwise use the prime test. */
        if (m == 1 || !is_prime_big(m)) {
            ++found;
            if (found == n) {
                return (seq_t)k;
            }
        }
    }
}
