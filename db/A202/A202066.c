/*
 * A202066  Mass of oriented maximal Wicks forms of genus n, multiplied by 6.
 * https://oeis.org/A202066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Catalan(n-1) * ∏_{k=1}^{n-1} ((6k-1)(6k+1))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   catalan, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Catalan of (n-1) */
    seq_t result = catalan(n - 1);

    /* Product over k from 1 to n-1 of (6k-1)(6k+1) */
    for (idx_t k = 1; k < n; ++k) {
        seq_t part = seq_mul((seq_t)(6 * k - 1), (seq_t)(6 * k + 1));
        result = seq_mul(result, part);
    }

    return result;
}
