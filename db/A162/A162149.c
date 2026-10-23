/*
 * A162149  Number of reduced words of length n in the Weyl group B_35.
 * https://oeis.org/A162149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1}^{35} (1 - x^(2k))/(1 - x)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    static seq_t coeff[1226];
    static seq_t tmp[1226];
    static bool initialized = false;

    if (!initialized) {
        for (idx_t i = 0; i <= 1225; ++i) coeff[i] = 0;
        coeff[0] = 1;
        idx_t cur_max = 0;

        for (idx_t k = 1; k <= 35; ++k) {
            idx_t max_add = 2 * k - 1;
            idx_t new_max = cur_max + max_add;
            for (idx_t i = 0; i <= new_max; ++i) tmp[i] = 0;

            for (idx_t i = 0; i <= cur_max; ++i) {
                seq_t v = coeff[i];
                if (v == 0) continue;
                for (idx_t add = 0; add <= max_add; ++add) {
                    tmp[i + add] += v;
                }
            }

            for (idx_t i = 0; i <= new_max; ++i) coeff[i] = tmp[i];
            cur_max = new_max;
        }

        initialized = true;
    }

    if (n <= 1225) return coeff[n];
    return 0;
}
