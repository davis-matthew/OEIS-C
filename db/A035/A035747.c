/*
 * A035747  Coordination sequence for C_10 lattice.
 * https://oeis.org/A035747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{i=0}^{10} binom(10,i) * binom(9 + 2*n - i, 9)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 11, u64 n <= 132,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here binom(a,b) is the binomial coefficient a choose b.  */
#include <stdint.h>

static const seq_t binom10[11] = {1,10,45,120,210,252,210,120,45,10,1};

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (int i = 0; i <= 10; ++i) {
        /* Skip terms where 9 + 2*n - i < 9 (i.e., binom is zero).  */
        if ((idx_t)i > 2 * n)
            break;

        seq_t coeff = binom10[i];
        /* Compute a_val = 9 + 2*n - i, which is always >= 9 here.  */
        seq_t a_val = (seq_t)9 + (seq_t)2 * (seq_t)n - (seq_t)i;

        /* Compute binom(a_val, 9) multiplicatively.  */
        seq_t bin9 = 1;
        for (int k = 1; k <= 9; ++k) {
            /* Avoid intermediate overflow for the 64‑bit build by performing
               division after each multiplication; this keeps the intermediate
               value small enough for all n where the result fits in seq_t.  */
            bin9 = bin9 * (a_val - (seq_t)(k - 1)) / (seq_t)k;
        }

        result += coeff * bin9;
    }
    return result;
}
