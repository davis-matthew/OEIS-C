/*
 * A018332  Divisors of 200.
 * https://oeis.org/A018332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 200: a(n) = the nth divisor of 200 (sorted increasingly) */
#include <stdbool.h>
#include <stddef.h>   /* for size_t */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* 200 = 2^3 * 5^2 has (3+1)(2+1)=12 divisors.  Generate and sort them once. */
    static seq_t divs[12];
    static bool init = false;

    if (!init) {
        const uint8_t pow2[4] = {1, 2, 4, 8};   /* 2^0 .. 2^3 */
        const uint8_t pow5[3] = {1, 5, 25};      /* 5^0 .. 5^2 */
        size_t idx = 0;

        for (size_t a = 0; a < 4; ++a) {
            for (size_t b = 0; b < 3; ++b) {
                divs[idx++] = (seq_t)pow2[a] * (seq_t)pow5[b];
            }
        }

        /* Simple insertion sort for the 12 elements */
        for (size_t i = 1; i < 12; ++i) {
            seq_t key = divs[i];
            size_t j = i;
            while (j > 0 && divs[j-1] > key) {
                divs[j] = divs[j-1];
                --j;
            }
            divs[j] = key;
        }
        init = true;
    }

    if (n >= 1 && (idx_t)n <= 12) {
        return divs[(size_t)n - 1];
    }
    return (seq_t)0;   /* undefined for n > 12 */
}
