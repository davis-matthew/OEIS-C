/*
 * A161693  Number of reduced words of length n in the Weyl group A_48.
 * https://oeis.org/A161693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1}^{48} (1 + x + … + x^k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static seq_t dp_final[1177];          /* holds the polynomial coefficients */

/* Build the polynomial once */
static void build_sequence(void)
{
    static bool built = false;
    if (built) return;

    static seq_t a[1177] = {0};
    static seq_t b[1177] = {0};
    a[0] = (seq_t)1;
    idx_t max_prev = 0;

    for (idx_t k = 1; k <= 48; ++k) {
        idx_t max_cur = max_prev + k;           /* degree after this factor */

        /* compute b[j] = sum_{t=0}^{k} a[j‑t] for 0 ≤ j ≤ max_cur */
        for (idx_t j = 0; j <= max_cur; ++j) {
            seq_t s = 0;
            idx_t tmax = (j < k) ? j : k;
            for (idx_t t = 0; t <= tmax; ++t) {
                s += a[j - t];
            }
            b[j] = s;
        }

        /* copy back to a for the next iteration */
        for (idx_t j = 0; j <= max_cur; ++j) a[j] = b[j];
        max_prev = max_cur;
    }

    /* final coefficients */
    for (idx_t i = 0; i < 1177; ++i) dp_final[i] = a[i];
    built = true;
}

seq_t a(idx_t n)
{
    if (n >= 1177) return (seq_t)0;  /* coefficient is zero beyond the degree */
    build_sequence();
    return dp_final[n];
}
