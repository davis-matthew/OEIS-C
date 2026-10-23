/*
 * A023624  Convolution of Lucas numbers and A014306.
 * https://oeis.org/A023624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{n} L_i * b_{n-i+1}, where L_i are Lucas numbers
 *            * (L_0 = 2, L_1 = 1, L_i = L_{i-1} + L_{i-2}) and * b_m = 0 if m
 *            is a tetrahedral number, 1 otherwise
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Precompute all tetrahedral numbers ≤ n */
    idx_t *tetra = NULL;
    idx_t tet_cnt = 0;
    for (idx_t k = 1;; ++k) {
        idx_t t = (k * (k + 1) * (k + 2)) / 6;   /* T_k */
        if (t > n)
            break;
        idx_t *tmp = (idx_t *)realloc(tetra, (tet_cnt + 1) * sizeof(idx_t));
        if (!tmp)
            abort();             /* allocation failure */
        tetra = tmp;
        tetra[tet_cnt++] = t;
    }

    seq_t total = 0;
    seq_t L_prev2 = (seq_t)2;  /* L_0 */
    seq_t L_prev1 = (seq_t)1;  /* L_1 */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t L_i;
        if (i == 1) {
            L_i = L_prev1;
        } else {
            L_i     = L_prev2 + L_prev1;
            L_prev2 = L_prev1;
            L_prev1 = L_i;
        }

        /* Complementary index j = n - i + 1 */
        idx_t j = n - i + 1;

        /* Check if j is a tetrahedral number */
        bool is_tetra = false;
        for (idx_t p = 0; p < tet_cnt; ++p) {
            if (tetra[p] == j) {
                is_tetra = true;
                break;
            }
            if (tetra[p] > j) /* tetra numbers are increasing */
                break;
        }

        if (!is_tetra)
            total += L_i;
    }

    free(tetra);
    return total;
}
