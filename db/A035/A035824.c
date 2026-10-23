/*
 * A035824  Coordination sequence for lattice D*_78 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{78} 2^k · C(78,k) · C(n-1,k-1) +
 *            (n≥39)·2^78·C(n+38,77), with a(0)=1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 13, u128 n <= 14,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binomial.h"

static seq_t coeff[79];          /* coeff[k] = 2^k * C(78,k), k=1..78 */
static bool coeff_initialized = false;

static void init_coeff(void)
{
    if (coeff_initialized) return;
    coeff_initialized = true;
    for (idx_t k = 1; k <= 78; ++k) {
        coeff[k] = pow2(k) * binomial(78, k);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    init_coeff();

    seq_t tot = 0;
    /* first part: sum_{k=1}^{78} coeff[k] * C(n-1, k-1)   */
    for (idx_t k = 1; k <= 78; ++k) {
        seq_t c = binomial(n - 1, k - 1);   /* C(n-1, k-1) */
        tot += coeff[k] * c;
    }

    /* second part: 2^78 * C(n+38, 77), added only when n≥39 */
    if (n >= 39) {
        seq_t bin = binomial(n + 38, 77);        /* C(n+38, 77) */
        tot += coeff[78] * bin;                  /* coeff[78] = 2^78 */
    }

    return tot;
}
