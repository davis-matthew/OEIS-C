/*
 * A073617  Consider Pascal's triangle A007318; a(n) = product of terms at +45
 *          degrees slope with the horizontal.
 * https://oeis.org/A073617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..⌊n/2⌋} binomial(n-k,k)
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 13, u64 n <= 18, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t kmax = n / 2;
    for (idx_t k = 0; k <= kmax; ++k) {
        seq_t term = binomial(n - k, k);
        result = result * term;
    }
    return result;
}
