/*
 * A162402  Number of reduced words of length n in the Weyl group D_40.
 * https://oeis.org/A162402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{d} (1 + x + … + x^{d-1}) where d
 *            = 40, 2, 4, …, 78 (40 factors)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>          /* for calloc, free */

static void multiply_by_factor(seq_t **coeff, idx_t *cur_deg,
                               idx_t max_deg, idx_t k)
{
    idx_t old_deg = *cur_deg;
    idx_t new_deg = old_deg + k - 1;
    if (new_deg > max_deg) new_deg = max_deg;

    seq_t *old = *coeff;
    seq_t *newc = (seq_t*)calloc((size_t)(max_deg + 1), sizeof(seq_t));

    for (idx_t i = 0; i <= old_deg; ++i) {
        seq_t val = old[i];
        for (idx_t offset = 0; offset < k; ++offset) {
            idx_t j = i + offset;
            if (j > max_deg) break;
            newc[j] += val;
        }
    }

    free(old);
    *coeff = newc;
    *cur_deg = new_deg;
}

seq_t a(idx_t n)
{
    const idx_t m      = 40;                /* rank of D_40 */
    const idx_t max_deg = m * (m - 1);      /* 1560 */

    static seq_t *coeff = NULL;
    static idx_t cur_deg = 0;
    static int   initialized = 0;

    if (n > max_deg) return 0;

    if (!initialized) {
        coeff = (seq_t*)calloc((size_t)(max_deg + 1), sizeof(seq_t));
        coeff[0] = 1;
        cur_deg = 0;

        /* first factor with d = 40 */
        multiply_by_factor(&coeff, &cur_deg, max_deg, m);

        /* remaining factors with d = 2, 4, …, 78 */
        for (idx_t i = 1; i < m; ++i) {
            multiply_by_factor(&coeff, &cur_deg, max_deg, 2 * i);
        }
        initialized = 1;
    }

    return coeff[n];
}
