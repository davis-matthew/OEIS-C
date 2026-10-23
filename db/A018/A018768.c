/*
 * A018768  Divisors of 1001.
 * https://oeis.org/A018768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th smallest divisor of 1001
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* precompute the divisors of 1001 only once */
    static bool initialized = false;
    static seq_t divisors[8];
    if (!initialized) {
        const uint64_t primes[3] = {7ULL, 11ULL, 13ULL};
        idx_t k = 0;
        /* enumerate all subsets of the three primes (exponents 0 or 1) */
        for (idx_t b0 = 0; b0 <= 1; ++b0)
            for (idx_t b1 = 0; b1 <= 1; ++b1)
                for (idx_t b2 = 0; b2 <= 1; ++b2) {
                    seq_t prod = 1;
                    if (b0) prod *= (seq_t)primes[0];
                    if (b1) prod *= (seq_t)primes[1];
                    if (b2) prod *= (seq_t)primes[2];
                    divisors[k++] = prod;   /* k goes from 0 to 7 */
                }
        /* simple bubble sort to order the divisors ascending */
        for (idx_t i = 0; i < 8; ++i)
            for (idx_t j = i + 1; j < 8; ++j)
                if (divisors[i] > divisors[j]) {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }
        initialized = true;
    }

    if (n < 1 || n > 8)
        return 0;
    return divisors[n - 1];
}
