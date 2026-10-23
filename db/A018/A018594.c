/*
 * A018594  Divisors of 690.
 * https://oeis.org/A018594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 690 in ascending order (1-indexed)
 * Range:     u8 n <= 11, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t divs[16];
    static bool initialized = false;
    if (!initialized) {
        const unsigned int primes[4] = {2U, 3U, 5U, 23U};
        const size_t prime_count = 4;
        size_t idx = 0;
        for (uint64_t mask = 0; mask < ((uint64_t)1 << prime_count); ++mask) {
            seq_t prod = (seq_t)1;
            for (size_t i = 0; i < prime_count; ++i) {
                if (mask & ((uint64_t)1 << i)) {
                    prod *= (seq_t)primes[i];
                }
            }
            divs[idx++] = prod;
        }
        /* simple insertion sort */
        for (size_t i = 1; i < 16; ++i) {
            seq_t key = divs[i];
            size_t j = i;
            while (j > 0 && divs[j-1] > key) {
                divs[j] = divs[j-1];
                --j;
            }
            divs[j] = key;
        }
        initialized = true;
    }
    if (n >= 1 && n <= 16) {
        return divs[(size_t)(n - 1)];
    }
    return (seq_t)0;
}
