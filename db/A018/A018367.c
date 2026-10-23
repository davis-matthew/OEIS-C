/*
 * A018367  Divisors of 273.
 * https://oeis.org/A018367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 273 in increasing order
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n) {
    static seq_t divisors[8];
    static bool init = false;

    if (!init) {
        const uint64_t primes[3] = {3, 7, 13};
        for (int mask = 0; mask < 8; ++mask) {
            seq_t val = 1;
            if (mask & 1) val *= (seq_t)primes[0];
            if (mask & 2) val *= (seq_t)primes[1];
            if (mask & 4) val *= (seq_t)primes[2];
            divisors[mask] = val;
        }
        /* sort ascending */
        for (int i = 0; i < 8; ++i) {
            for (int j = i + 1; j < 8; ++j) {
                if (divisors[i] > divisors[j]) {
                    seq_t t = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = t;
                }
            }
        }
        init = true;
    }

    if (n == 0) return 0;
    if (n <= 8) return divisors[n - 1];
    return 0;
}
