/*
 * A161699  Number of reduced words of length n in the Weyl group B_6.
 * https://oeis.org/A161699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1..6} (1 + x + … + x^{2k-1})
 * Range:     u8 n <= 5, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n) {
    static seq_t coeff[37] = {0};
    static bool initialized = false;
    if (!initialized) {
        coeff[0] = 1;
        size_t deg = 0;
        for (idx_t k = 1; k <= 6; ++k) {
            size_t len = 2 * (size_t)k;          /* number of terms in the k‑th factor */
            seq_t next[37];
            for (size_t i = 0; i <= 36; ++i) next[i] = 0;   /* clear next array */
            for (size_t i = 0; i <= deg; ++i) {
                seq_t v = coeff[i];
                for (size_t j = 0; j < len; ++j) {
                    next[i + j] += v;
                }
            }
            deg += len - 1;                      /* new degree after multiplication */
            for (size_t i = 0; i <= deg; ++i) coeff[i] = next[i];
        }
        initialized = true;
    }

    if (n <= 36) return coeff[(size_t)n];
    return 0;
}
