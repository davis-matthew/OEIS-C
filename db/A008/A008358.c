/*
 * A008358  Crystal ball sequence for D_6 lattice.
 * https://oeis.org/A008358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σₖ₌₀⁶ c_k * binomial(n - k + 6, 6)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 33, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where c = {1, 54, 447, 852, 447, 54, 1}                                            */

#include "../utils/binomial.h"

static const seq_t coeffs[7] = {1, 54, 447, 852, 447, 54, 1};

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k < 7; ++k) {
        if (n < k) continue;
        idx_t m = n - k + 6;           /* n - k + 6 >= 6 by construction */
        seq_t b   = binomial(m, 6);    /* coefficient of (1 - x)^-7  */
        sum += coeffs[k] * b;
    }
    return sum;
}
