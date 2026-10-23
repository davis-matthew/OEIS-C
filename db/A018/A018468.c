/*
 * A018468  Divisors of 462.
 * https://oeis.org/A018468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 462 in ascending order
 * Range:     u8 n <= 14, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n) {
    static bool ready = false;
    static size_t cnt = 0;
    static seq_t divs[16];

    if (!ready) {
        /* generate all divisors of 462 = 2 * 3 * 7 * 11 */
        const uint64_t primes[4] = {2, 3, 7, 11};
        const size_t primes_cnt = 4;
        cnt = 0;

        for (uint64_t mask = 0; mask < ((uint64_t)1 << primes_cnt); ++mask) {
            seq_t prod = 1;
            for (size_t i = 0; i < primes_cnt; ++i) {
                if (mask & ((uint64_t)1 << i))
                    prod = prod * primes[i];
            }
            divs[cnt++] = prod;
        }

        /* sort ascending (simple insertion sort) */
        for (size_t i = 0; i < cnt; ++i) {
            for (size_t j = i + 1; j < cnt; ++j) {
                if (divs[i] > divs[j]) {
                    seq_t tmp = divs[i];
                    divs[i] = divs[j];
                    divs[j] = tmp;
                }
            }
        }

        ready = true;
    }

    if (n >= 1 && (size_t)n <= cnt) {
        return divs[n - 1];
    }
    return (seq_t)0;
}
