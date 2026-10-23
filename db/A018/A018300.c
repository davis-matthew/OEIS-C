/*
 * A018300  Divisors of 138.
 * https://oeis.org/A018300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 138, sorted in ascending order; returns 0
 *            for n > 8
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t divisors[8];
    static bool ready = false;

    if (!ready) {
        /* generate all divisors of 138 = 2 * 3 * 23 */
        const uint64_t primes[3] = {2, 3, 23};
        size_t count = 0;
        for (uint64_t mask = 0; mask < ((uint64_t)1 << 3); ++mask) {
            seq_t val = 1;
            for (size_t i = 0; i < 3; ++i) {
                if (mask & ((uint64_t)1 << i))
                    val = val * (seq_t)primes[i];
            }
            divisors[count++] = val;
        }
        /* simple bubble sort for the 8 elements */
        for (size_t i = 0; i < 8; ++i) {
            for (size_t j = i + 1; j < 8; ++j) {
                if (divisors[j] < divisors[i]) {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }
            }
        }
        ready = true;
    }

    if (n < 1 || n > 8)
        return 0;           /* undefined for indices beyond the finite sequence */
    return divisors[(size_t)(n - 1)];
}
