/*
 * A119367  Number of rooted planar n-trees whose number of leaves is equal to
 *          2 modulo 3.
 * https://oeis.org/A119367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=0..n, if (n-k) mod 3 = 2} (1/n) * C(n,k) * C(n,k+1)
 *            * where C(n,k) is the binomial coefficient
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binom_n_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_n_k.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* Base case: a(0) = 0 */
    seq_t result = 0;
    for (idx_t k = 0; k <= n; ++k) {
        if ((n - k) % 3 != 2) continue;
        seq_t c1 = binom_n_k(n, k);
        seq_t c2 = binom_n_k(n, k + 1); /* k+1 <= n for these k */
        seq_t prod = c1 * c2;
        prod = prod / ((seq_t)n);       /* exact division */
        result += prod;
    }
    return result;
}
