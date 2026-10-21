/*
 * A084876  Number of (k,m,n)-antichains of multisets with k=3 and m=4.
 * https://oeis.org/A084876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (81^n - 12*54^n + 24*42^n + 4*36^n - 24*31^n + 6*27^n +
 *            6*26^n - 36*18^n + 36*14^n + 11*9^n - 22*6^n + 6*3^n) / 24
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/big_pow.h"  /* big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    /* Bases used in the formula */
    static const idx_t bases[12] = {
        81U, 54U, 42U, 36U, 31U,
        27U, 26U, 18U, 14U, 9U, 6U, 3U
    };

    /* Corresponding coefficients (positive or negative) */
    static const int coeffs[12] = {
        +1, -12, +24, +4, -24,
        +6, +6, -36, +36, +11, -22, +6
    };

    seq_t pos_sum = 0;   /* sum of terms with positive coefficient   */
    seq_t neg_sum = 0;   /* sum of terms with negative coefficient   */

    for (idx_t i = 0; i < (idx_t)12; ++i) {
        seq_t p = big_pow((seq_t)bases[i], (seq_t)n); /* compute base^n */

        if (coeffs[i] > 0) {
            pos_sum += p * (unsigned)coeffs[i];
        } else {
            neg_sum += p * (unsigned)(-coeffs[i]);
        }
    }

    /* total = pos_sum - neg_sum; the result is guaranteed non‑negative   */
    seq_t total = pos_sum;
    total -= neg_sum;

    /* divide by 24 (4!) */
    return total / ((seq_t)24);
}
