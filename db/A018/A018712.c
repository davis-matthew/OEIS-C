/*
 * A018712  Divisors of 903.
 * https://oeis.org/A018712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 903 sorted increasingly; a(n) = 0 for n >
 *            8
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t divisors[8];
    static bool computed = false;

    if (!computed) {
        /* prime factors of 903: 3, 7, 43 */
        const uint64_t primes[3] = {3, 7, 43};
        const int pcount = 3;

        /* generate all 2^3 divisors */
        for (idx_t mask = 0; mask < (idx_t)(1u << pcount); ++mask) {
            seq_t prod = 1;
            for (int i = 0; i < pcount; ++i) {
                if (mask & ((idx_t)1 << i))
                    prod *= primes[i];
            }
            divisors[mask] = prod;
        }

        /* simple bubble sort (8 elements) */
        for (int i = 0; i < 7; ++i) {
            for (int j = 0; j < 6 - i; ++j) {
                if (divisors[j] > divisors[j + 1]) {
                    seq_t tmp = divisors[j];
                    divisors[j] = divisors[j + 1];
                    divisors[j + 1] = tmp;
                }
            }
        }
        computed = true;
    }

    if (n > 8)
        return 0;
    return divisors[n - 1];
}
