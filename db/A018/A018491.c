/*
 * A018491  Divisors of 506.
 * https://oeis.org/A018491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 506 in ascending order (finite
 *            sequence)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* 506 = 2 × 11 × 23, so there are 8 divisors. */
    static seq_t divs[8];
    static int initialized = 0;

    if (!initialized) {
        const uint64_t primes[3] = {2, 11, 23};
        size_t idx = 0;

        for (uint8_t mask = 0; mask < 8u; ++mask) {
            seq_t val = 1;
            for (int j = 0; j < 3; ++j) {
                if (mask & (1u << j))
                    val *= (seq_t)primes[j];
            }
            divs[idx++] = val;
        }

        /* Sort the 8 values in ascending order. */
        for (size_t i = 1; i < 8; ++i) {
            seq_t key = divs[i];
            size_t j = i;
            while (j > 0 && divs[j - 1] > key) {
                divs[j] = divs[j - 1];
                --j;
            }
            divs[j] = key;
        }

        initialized = 1;
    }

    if (n < 1 || n > 8)
        return 0; /* undefined for indices outside the divisor list */

    return divs[(size_t)(n - 1)];
}
