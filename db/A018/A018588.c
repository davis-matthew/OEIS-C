/*
 * A018588  Divisors of 678.
 * https://oeis.org/A018588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 678 in increasing order
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The divisors of 678 = 2 * 3 * 113 are 1, 2, 3, 6, 113, 226, 339, 678. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1ull)                      /* undefined for n < 1 */
        return (seq_t)0;

    static bool initialized = false;
    static seq_t divs[8];

    if (!initialized) {
        idx_t pos = 0ull;
        for (idx_t i = 0ull; i <= 1ull; ++i) {
            for (idx_t j = 0ull; j <= 1ull; ++j) {
                for (idx_t k = 0ull; k <= 1ull; ++k) {
                    seq_t v = (seq_t)1;
                    if (i) v *= (seq_t)2;
                    if (j) v *= (seq_t)3;
                    if (k) v *= (seq_t)113;
                    divs[pos++] = v;
                }
            }
        }
        /* Insertion sort – the array is only 8 elements long. */
        for (idx_t i = 1ull; i < 8ull; ++i) {
            seq_t key = divs[i];
            idx_t j = i;
            while (j > 0ull && divs[j-1ull] > key) {
                divs[j] = divs[j-1ull];
                --j;
            }
            divs[j] = key;
        }
        initialized = true;
    }

    if (n > 8ull) return (seq_t)0;     /* no further divisors defined */
    return divs[n-1ull];
}
