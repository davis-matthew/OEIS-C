/*
 * A018429  Divisors of 390.
 * https://oeis.org/A018429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 390 in ascending order */
#include <stddef.h>

seq_t a(idx_t n) {
    static seq_t divisors[16];
    static size_t count = 0;

    if (count == 0) {
        /* Generate all 16 divisors. The prime factors of 390 are
           2, 3, 5 and 13. Each can be present (exponent 1) or not
           (exponent 0). */
        const seq_t primes[4] = {2, 3, 5, 13};
        int i = 0;
        for (int mask = 0; mask < 16; ++mask) {
            seq_t d = 1;
            if (mask & 1) d *= primes[0];
            if (mask & 2) d *= primes[1];
            if (mask & 4) d *= primes[2];
            if (mask & 8) d *= primes[3];
            divisors[i++] = d;
        }
        count = 16;

        /* Simple insertion sort to obtain ascending order. */
        for (size_t i = 0; i < count; ++i) {
            for (size_t j = i + 1; j < count; ++j) {
                if (divisors[j] < divisors[i]) {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }
            }
        }
    }

    if (n >= 1 && n <= count)
        return divisors[n - 1];
    else
        return (seq_t)0;   /* Undefined beyond the last divisor */
}
