/*
 * A161662  Number of reduced words of length n in the Weyl group A_41.
 * https://oeis.org/A161662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1..41}(1 + x + … + x^k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of reduced words of length n in the Weyl group A_41 (S_42). */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* Precomputed polynomial coefficients up to degree 861
       (1 + x + … + x^k) for k = 1..41. */
    static bool initialized = false;
    static seq_t coeff[862] = {0};

    if (!initialized) {
        const idx_t m = 41;
        const idx_t max_n = 861;          /* 1+2+…+41 */
        coeff[0] = 1;                     /* Start with polynomial 1 */

        for (idx_t k = 1; k <= m; ++k) {
            seq_t next[862] = {0};

            for (idx_t i = 0; i <= max_n; ++i) {
                seq_t val = coeff[i];
                if (val == 0) continue;   /* skip zero terms */

                /* Multiply by (1 + x + … + x^k) */
                for (idx_t e = 0; e <= k; ++e) {
                    if (i + e > max_n) break;
                    next[i + e] += val;
                }
            }
            /* Copy back to coeff for next iteration */
            for (idx_t i = 0; i <= max_n; ++i)
                coeff[i] = next[i];
        }
        initialized = true;
    }

    return (n <= 861) ? coeff[n] : (seq_t)0;
}
