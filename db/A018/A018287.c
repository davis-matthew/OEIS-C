/*
 * A018287  Divisors of 108.
 * https://oeis.org/A018287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   nth divisor of 108 in ascending order
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    static seq_t divs[12];
    static bool init = false;

    if (!init) {
        seq_t pow2[3] = { (seq_t)1, (seq_t)2, (seq_t)4 };
        seq_t pow3[4] = { (seq_t)1, (seq_t)3, (seq_t)9, (seq_t)27 };
        idx_t k = 0;
        for (idx_t i = 0; i < 3; ++i) {
            seq_t v2 = pow2[i];
            for (idx_t j = 0; j < 4; ++j) {
                divs[k++] = v2 * pow3[j];
            }
        }
        /* simple insertion sort to obtain ascending order */
        for (idx_t i = 1; i < 12; ++i) {
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

    if (n < 1 || n > 12) return (seq_t)0;
    return divs[n - 1];
}
