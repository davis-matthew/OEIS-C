/*
 * A081197  Diagonal sums of A081130.
 * https://oeis.org/A081197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n-2} k^(n-k-2)*C(n-k,2)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 17, u64 n <= 28, u128 n <= 46,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;
    seq_t total = 0;
    for (idx_t k = 1; k <= n - 2; ++k) {
        idx_t nk = n - k;                       /* n-k >= 2 */
        seq_t bin = binomial(nk, 2);            /* C(n-k, 2) */
        seq_t exp = (seq_t)(nk - 2);            /* n-k-2 */
        seq_t pw  = big_pow((seq_t)k, exp);     /* k^(n-k-2) */
        total += pw * bin;
    }
    return total;
}
