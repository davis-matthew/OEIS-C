/*
 * A018695  Divisors of 873.
 * https://oeis.org/A018695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 873 in ascending order (computed by divisor search) */
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache of the six divisors of 873 */
    static bool initialized = false;
    static seq_t divs[16];
    static size_t divcnt = 0;

    if (!initialized) {
        /* compute all divisors of 873 */
        const idx_t N = 873ULL;
        for (idx_t d = 1; d * d <= N; ++d) {
            if (N % d == 0) {
                if (divcnt < sizeof(divs) / sizeof(divs[0]))
                    divs[divcnt++] = (seq_t)d;
                if (d != N / d) {
                    if (divcnt < sizeof(divs) / sizeof(divs[0]))
                        divs[divcnt++] = (seq_t)(N / d);
                }
            }
        }
        /* sort the small array (insertion sort, size <= 6) */
        for (size_t i = 1; i < divcnt; ++i) {
            seq_t key = divs[i];
            size_t j = i;
            while (j > 0 && divs[j - 1] > key) {
                divs[j] = divs[j - 1];
                --j;
            }
            divs[j] = key;
        }
        initialized = true;
    }

    if (n == 0 || n > divcnt)
        return (seq_t)0;
    return divs[(size_t)n - 1];
}
