/*
 * A105039  Number of compositions of n with unique smallest part.
 * https://oeis.org/A105039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + Σ_{k=2}^{⌊(n+3)/2⌋} k * Σ_{s=1}^{⌊(n-1)/k⌋}
 *            binomial(n - k*s - 1, k-2)
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 44, u64 n <= 89, u128 n <= 179,
 *            big any n
 * Helpers:   binomial, mul128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/mul128.h"

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    seq_t result = 1;                     /* a(1) = 1 */
    idx_t k_limit = (n + 3) / 2;           /* upper bound for k */
    for (idx_t k = 2; k <= k_limit; ++k) {
        seq_t inner_sum = 0;
        idx_t s_limit = (n - 1) / k;      /* upper bound for s */
        for (idx_t s = 1; s <= s_limit; ++s) {
            idx_t N = n - k * s - 1;
            idx_t K = k - 2;
            seq_t bin = binomial(N, K);
            inner_sum += bin;
        }
        seq_t addend = mul128((seq_t)k, inner_sum);
        result += addend;
    }
    return result;
}
