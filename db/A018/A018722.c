/*
 * A018722  Divisors of 920.
 * https://oeis.org/A018722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the n-th divisor of 920 in increasing order
 * Range:     u8 n <= 12, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* 920 = 2^3 · 5 · 23  →  (3+1)(1+1)(1+1) = 16 divisors. */
    static seq_t divs[20];
    static const idx_t count = 16;
    static bool init = false;

    if (!init) {
        idx_t pos = 0;
        /* Loop over all exponent choices to generate each divisor. */
        for (idx_t e2 = 0; e2 <= 3; ++e2) {
            seq_t p2 = 1;
            for (idx_t k = 0; k < e2; ++k) p2 *= 2;
            for (idx_t e5 = 0; e5 <= 1; ++e5) {
                seq_t p5 = 1;
                for (idx_t k = 0; k < e5; ++k) p5 *= 5;
                for (idx_t e23 = 0; e23 <= 1; ++e23) {
                    seq_t v = p2 * p5;
                    for (idx_t k = 0; k < e23; ++k) v *= 23;
                    divs[pos++] = v;
                }
            }
        }
        /* Simple insertion sort (16 elements only). */
        for (idx_t i = 1; i < pos; ++i) {
            seq_t key = divs[i];
            idx_t j = i;
            while (j > 0 && divs[j - 1] > key) {
                divs[j] = divs[j - 1];
                --j;
            }
            divs[j] = key;
        }
        init = true;
    }

    if (n == 0 || n > count) return (seq_t)0;
    return divs[(idx_t)(n - 1)];
}
