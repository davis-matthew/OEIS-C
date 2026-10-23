/*
 * A018707  Divisors of 894.
 * https://oeis.org/A018707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor (in ascending order) of 894
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 894 = 2 · 3 · 149, so it has exactly 8 positive divisors. */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t divisors[8];
    static bool initialized = false;

    if (!initialized) {
        /* prime factorization of 894 */
        const uint64_t primes[3] = { 2, 3, 149 };
        seq_t tmp[8];
        idx_t cnt = 0;

        /* generate all products of subsets of the primes */
        for (idx_t mask = 0; mask < (1ULL << 3); ++mask) {
            seq_t prod = 1;
            for (idx_t i = 0; i < 3; ++i) {
                if (mask & (1ULL << i))
                    prod *= (seq_t)primes[i];
            }
            tmp[cnt++] = prod;
        }

        /* bubble sort the small array */
        for (idx_t i = 0; i < 7; ++i) {
            for (idx_t j = 0; j < 7 - i; ++j) {
                if (tmp[j] > tmp[j + 1]) {
                    seq_t t = tmp[j];
                    tmp[j] = tmp[j + 1];
                    tmp[j + 1] = t;
                }
            }
        }

        for (idx_t i = 0; i < 8; ++i)
            divisors[i] = tmp[i];

        initialized = true;
    }

    if (n == 0 || n > 8)
        return 0;
    return divisors[(size_t)(n - 1)];
}
