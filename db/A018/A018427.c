/*
 * A018427  Divisors of 387.
 * https://oeis.org/A018427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the n-th divisor of 387, sorted in increasing order
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Precompute all divisors of 387 = 3^2 * 43 once. */
    static seq_t divs[6];
    static bool inited = false;
    if (!inited) {
        size_t idx = 0;
        for (int e3 = 0; e3 <= 2; ++e3) {
            seq_t v3 = 1;
            for (int k = 0; k < e3; ++k) v3 *= 3;
            for (int e43 = 0; e43 <= 1; ++e43) {
                seq_t val = v3;
                for (int k = 0; k < e43; ++k) val *= 43;
                divs[idx++] = val;
            }
        }
        /* Sort the six values in increasing order (simple bubble sort). */
        for (size_t i = 0; i < 6; ++i) {
            for (size_t j = i + 1; j < 6; ++j) {
                if (divs[j] < divs[i]) {
                    seq_t tmp = divs[i];
                    divs[i] = divs[j];
                    divs[j] = tmp;
                }
            }
        }
        inited = true;
    }

    if (n <= 6) return divs[n - 1];
    return 0;  /* undefined for n > number of divisors */
}
