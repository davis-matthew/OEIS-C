/*
 * A018401  Divisors of 340.
 * https://oeis.org/A018401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 340:  a(n) is the n‑th divisor of 340 in ascending order.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1 || n > 12)   /* sequence has exactly 12 terms */
        return 0;

    static seq_t divs[12];
    static int init = 0;

    if (!init) {
        size_t idx = 0;
        for (int e2 = 0; e2 <= 2; ++e2) {                /* exponent of 2 */
            seq_t p2 = 1;
            for (int i = 0; i < e2; ++i) p2 *= 2;
            for (int e5 = 0; e5 <= 1; ++e5) {            /* exponent of 5 */
                seq_t p5 = 1;
                for (int i = 0; i < e5; ++i) p5 *= 5;
                for (int e17 = 0; e17 <= 1; ++e17) {     /* exponent of 17 */
                    seq_t p17 = 1;
                    for (int i = 0; i < e17; ++i) p17 *= 17;
                    divs[idx++] = p2 * p5 * p17;
                }
            }
        }

        /* Simple bubble sort – works for any integral seq_t. */
        for (size_t i = 0; i < 12; ++i) {
            for (size_t j = i + 1; j < 12; ++j) {
                if (divs[j] < divs[i]) {
                    seq_t tmp = divs[i];
                    divs[i] = divs[j];
                    divs[j] = tmp;
                }
            }
        }
        init = 1;
    }

    return divs[n - 1];
}
