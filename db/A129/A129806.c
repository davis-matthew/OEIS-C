/*
 * A129806  Primes congruent to +-5 mod 18.
 * https://oeis.org/A129806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th prime such that p % 18 == 5 or 13
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isPrimeGeneric.h"
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;

    idx_t found = 0;
    seq_t cand = 5;          /* first candidate (5 is the first term) */

    while (found < n) {
        if ((cand % 18 == 5) || (cand % 18 == 13)) {
            if (isPrimeGeneric(cand)) {
                ++found;
                if (found == n) return cand;
            }
        }
        cand += 2;           /* only odd numbers */
    }

    return 0; /* unreachable, but keeps the compiler happy */
}
