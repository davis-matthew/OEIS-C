/*
 * A161879  Number of reduced words of length n in the Weyl group B_19.
 * https://oeis.org/A161879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of ∏_{k=1..19} (1 + x + … + x^{2k‑1}) in the
 *            expanded polynomial. This is the number of reduced words of
 *            length n in the Weyl group B₁₉
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 17, u64 n <= 101, u128 n <= 361,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define M      19
#define DEG    (M * M)            /* 361 */
#define SIZE   (DEG + 1)          /* 362 */

seq_t a(idx_t n)
{
    static seq_t coeff[SIZE];
    static seq_t tmp[SIZE];
    static bool   initialized = false;
    static idx_t length = 0;       /* current polynomial length */

    if (!initialized) {
        /* start with polynomial 1 */
        coeff[0] = 1;
        length   = 1;

        for (idx_t k = 1; k <= M; ++k) {
            idx_t factor_len = 2 * k;           /* length of (1 + x + … + x^{2k-1}) */
            idx_t new_len    = length + factor_len - 1;

            /* clear temporary buffer */
            for (idx_t i = 0; i < SIZE; ++i)
                tmp[i] = 0;

            /* convolution: tmp = coeff * factor */
            for (idx_t i = 0; i < length; ++i) {
                seq_t c = coeff[i];
                for (idx_t j = 0; j < factor_len; ++j)
                    tmp[i + j] += c;
            }

            /* copy back to coeff */
            for (idx_t i = 0; i < new_len; ++i)
                coeff[i] = tmp[i];

            length = new_len;
        }

        initialized = true;
    }

    /* out of range → coefficient 0 */
    if (n >= length)
        return 0;

    return coeff[n];
}
