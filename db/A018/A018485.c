/*
 * A018485  Divisors of 494.
 * https://oeis.org/A018485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 494 in increasing order
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* Static cache for the divisors of 494 */
    static seq_t divisors[64];
    static size_t divisor_count;
    static bool initialized = false;

    if (!initialized) {
        /* Factor 494 */
        uint64_t m = 494ULL;
        uint64_t primes[10];
        size_t exps[10];
        size_t pcnt = 0;

        for (uint64_t p = 2; p * p <= m; ++p) {
            size_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            if (e > 0) {
                primes[pcnt] = p;
                exps[pcnt] = e;
                ++pcnt;
            }
        }
        if (m > 1) {
            primes[pcnt] = m;
            exps[pcnt] = 1;
            ++pcnt;
        }

        /* Compute total number of divisors */
        divisor_count = 1;
        for (size_t i = 0; i < pcnt; ++i)
            divisor_count *= (exps[i] + 1);

        /* Generate all divisors */
        for (size_t mask = 0; mask < (1ULL << pcnt); ++mask) {
            seq_t prod = 1;
            for (size_t i = 0; i < pcnt; ++i) {
                if (mask & (1ULL << i)) {
                    for (size_t e = 0; e < exps[i]; ++e)
                        prod *= (seq_t)primes[i];
                }
            }
            divisors[mask] = prod;
        }

        /* Sort divisors in ascending order */
        for (size_t i = 1; i < divisor_count; ++i) {
            seq_t key = divisors[i];
            int j = (int)i - 1;
            while (j >= 0 && divisors[(size_t)j] > key) {
                divisors[(size_t)(j + 1)] = divisors[(size_t)j];
                --j;
            }
            divisors[(size_t)(j + 1)] = key;
        }

        initialized = true;
    }

    if (n == 0 || n > divisor_count)
        return 0;  /* undefined for out-of-range requests */

    return divisors[(size_t)(n - 1)];
}
