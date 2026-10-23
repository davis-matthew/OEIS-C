/*
 * A104880  Diagonal sums of a sum-of-powers triangle.
 * https://oeis.org/A104880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n + sum_{k=2..⌊n/2⌋} (k^(n-2k+1)-1)/(k-1), a(0)=1, a(1)=1
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;                 /* a(0) = 1, a(1) = 1 */

    seq_t result = (seq_t)n;                     /* start with the term n */
    idx_t half = n / 2;

    for (idx_t k = 2; k <= half; ++k) {
        idx_t exp = n - 2*k + 1;                  /* exponent > 0 for n ≥ 2 */
        seq_t power = seq_pow((seq_t)k, exp);     /* k^exp  */
        seq_t term = (power - (seq_t)1) / ((seq_t)(k - 1));
        result += term;
    }

    return result;
}
