/*
 * A104879  Row sums of a sum-of-powers triangle.
 * https://oeis.org/A104879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + n + Sum_{k=2..n+1} (k^(n-k+1)-1)/(k-1)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   sum_powers, sum_powers_recursive
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sum_powers.h"
#include "../utils/sum_powers_recursive.h"
/* This implements the sum of powers in a recursive, O(n log n) manner. */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t total = (seq_t)0;
    for (idx_t k = 2; k <= n; ++k) {
        idx_t exp = n - k + 1;         /* exp ≥ 1 for k ≤ n */
        seq_t sum_k = sum_powers((seq_t)k, exp);
        total += sum_k;
    }
    return (seq_t)1 + (seq_t)n + total;
}
