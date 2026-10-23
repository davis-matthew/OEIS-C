/*
 * A162191  Number of reduced words of length n in the Weyl group B_47.
 * https://oeis.org/A162191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = coefficient of x^n in the product
 *                ∏_{k=1..47} (1 + x + … + x^(2k-1))
 * i.e. the generating function for the number of reduced words
 * of length n in the Weyl group B_{47}.
 */
#include <stdbool.h>

static seq_t coeff[47 * 47 + 1];   /* 2210 coefficients */
static bool built = false;

static void build(void)
{
    if (built) return;
    const idx_t m = 47;
    idx_t max_deg = 0;                     /* current maximum degree */
    for (idx_t i = 0; i <= max_deg; ++i) coeff[i] = 0;
    coeff[0] = 1;                          /* empty word */

    for (idx_t k = 1; k <= m; ++k) {
        const idx_t maxk = 2 * k - 1;      /* length of the k-th factor */
        const idx_t old_max = max_deg;
        max_deg += maxk;                  /* new maximum degree */

        /* Update coefficients in reverse to avoid using newly added values */
        for (idx_t i = old_max; i <= old_max; --i) {
            if (coeff[i] == 0) continue;
            seq_t val = coeff[i];
            for (idx_t t = 1; t <= maxk; ++t) {
                coeff[i + t] += val;
            }
            if (i == 0) break;
        }
    }
    built = true;
}

seq_t a(idx_t n)
{
    build();
    if (n > 47 * 47) return 0;
    return coeff[n];
}
