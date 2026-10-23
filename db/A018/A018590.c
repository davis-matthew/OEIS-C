/*
 * A018590  Divisors of 682.
 * https://oeis.org/A018590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 682 in ascending order
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    static seq_t divisors[8];
    static bool init = false;

    if (!init) {
        const seq_t primes[3] = { 2, 11, 31 };
        /* generate all 8 divisors (2^3 combos) */
        for (idx_t mask = 0; mask < 8; ++mask) {
            seq_t prod = 1;
            for (idx_t i = 0; i < 3; ++i) {
                if (mask & (1ull << i)) {
                    prod = prod * primes[i];
                }
            }
            divisors[mask] = prod;
        }
        /* sort ascending */
        for (idx_t i = 0; i < 7; ++i) {
            for (idx_t j = i + 1; j < 8; ++j) {
                if (divisors[j] < divisors[i]) {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }
            }
        }
        init = true;
    }

    if (n <= 8) return divisors[n - 1];
    return 0;
}
