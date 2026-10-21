/*
 * A057674  Primes -p+2^n with smallest p prime, arising in A057674.
 * https://oeis.org/A057674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2^(n+1) - p, where p is the smallest prime such that the
 *            result (2^(n+1) - p) is prime. Negative primes are allowed
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 30, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   isPrimeGeneric, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* Compute 2^(n+1). */
    seq_t N = pow2(n + 1);

    /* Search for the smallest prime p with prime difference. */
    for (idx_t p = 2; ; ++p) {
        if (!isPrimeGeneric((seq_t)p))
            continue;

        seq_t diff = N - (seq_t)p;

        if (diff < 0) {
            if (isPrimeGeneric(-(diff)))
                return diff;
        } else {
            if (isPrimeGeneric(diff))
                return diff;
        }
    }
}
