/*
 * A018548  Divisors of 609.
 * https://oeis.org/A018548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence enumerates the divisors of 609 in increasing order. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t divisors[8];
    static bool built = false;

    if (!built) {
        /* Prime factorization of 609: 3 * 7 * 29. */
        const uint64_t primes[3] = {3, 7, 29};
        idx_t mask;
        idx_t i;

        /* Generate all 2^3 = 8 divisors by iterating over subsets of the
           prime factors. */
        for (mask = 0; mask < (1ULL << 3); ++mask) {
            seq_t prod = 1;
            if (mask & 1ULL) prod *= (seq_t)primes[0];
            if (mask & 2ULL) prod *= (seq_t)primes[1];
            if (mask & 4ULL) prod *= (seq_t)primes[2];
            divisors[mask] = prod;
        }

        /* Sort the 8 divisors in ascending order (insertion sort, constant size). */
        for (i = 1; i < 8; ++i) {
            seq_t key = divisors[i];
            idx_t j = i;
            while (j > 0 && divisors[j - 1] > key) {
                divisors[j] = divisors[j - 1];
                --j;
            }
            divisors[j] = key;
        }

        built = true;
    }

    /* Return the appropriate term or 0 if n is outside the defined range. */
    return (n >= 1 && n <= 8) ? divisors[n - 1] : (seq_t)0;
}
