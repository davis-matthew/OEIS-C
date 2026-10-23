/*
 * A035813  Coordination sequence for lattice D*_56 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = add(2^k * binomial(56, k) * binomial(n-1, k-1), k=1..56)
 *            + 2^56 * binomial(n+27, 55)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

static seq_t coeff[57];
static int coeff_initialized = 0;
static seq_t pow2_56;

static void init_coeff(void)
{
    if (coeff_initialized) return;
    for (idx_t k = 0; k <= 56; ++k) {
        coeff[k] = pow2(k) * binomial(56, k);
    }
    pow2_56 = pow2(56);
    coeff_initialized = 1;
}

seq_t a(idx_t n)
{
    init_coeff();
    if (n == 0) return (seq_t)1;
    seq_t sum = 0;
    for (idx_t k = 1; k <= 56 && k <= n; ++k) {
        seq_t b = binomial(n - 1, k - 1);
        if (b == 0) continue;
        sum += coeff[k] * b;
    }
    seq_t term2 = pow2_56 * binomial(n + 27, 55);
    sum += term2;
    return sum;
}
