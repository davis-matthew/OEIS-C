/*
 * A162166  Number of reduced words of length n in the Weyl group B_38.
 * https://oeis.org/A162166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in Prod_{k=1..38} (1 + x + … +
 *            x^(2k-1))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The polynomial has degree 38^2 = 1444; only the first 1445 coefficients are non‑zero. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache of the polynomial coefficients */
    static seq_t *coeff = NULL;
    const idx_t M = 38;
    const size_t MAX_DEGREE = (size_t)M * (size_t)M;      /* 1444 */

    if (coeff == NULL) {
        /* Build the polynomial by successive convolution with (1+x+…+x^(2k-1)) */
        size_t cur_len = 1;
        seq_t *cur = (seq_t*)malloc(cur_len * sizeof(seq_t));
        if (!cur) return 0;
        cur[0] = 1;                 /* polynomial 1 */

        for (idx_t k = 1; k <= M; ++k) {
            size_t L = 2 * (size_t)k;           /* length of the next factor (all ones) */
            size_t new_len = cur_len + L - 1;    /* degree grows by (L-1)  */
            seq_t *next = (seq_t*)malloc(new_len * sizeof(seq_t));
            if (!next) { free(cur); return 0; }

            seq_t window = 0;
            for (size_t i = 0; i < new_len; ++i) {
                if (i < cur_len)         window += cur[i];
                if (i >= L)               window -= cur[i - L];
                next[i] = window;
            }

            free(cur);
            cur     = next;
            cur_len = new_len;
        }
        coeff = cur;   /* coeff[0 .. MAX_DEGREE] holds the desired values */
    }

    if ((size_t)n > MAX_DEGREE) return 0;
    return coeff[(size_t)n];
}
