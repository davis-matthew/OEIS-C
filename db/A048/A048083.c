/*
 * A048083  Number of nonempty subsets of {1,2,...,n} in which exactly 1/3 of
 *          the elements are <= (n+3)/3.
 * https://oeis.org/A048083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=1}^{⌊min(|A|,|B|/2)⌋} C(|A|,k) * C(|B|,2k) where A =
 *            {1,…,⌊(n+3)/3⌋}, B = {⌊(n+3)/3⌋+1,…,n}
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h> /* if needed */

static seq_t binom_basic(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;         /* use symmetry */
    seq_t result = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        seq_t numerator   = (seq_t)(n - k + i);
        seq_t denominator = (seq_t)i;
        result = result * numerator / denominator;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* under the offset n=1 is first defined term */

    /* compute |A| and |B| */
    idx_t a_size = (n + 3) / 3;       /* ⌊(n+3)/3⌋  */
    idx_t b_size = n - a_size;

    /* maximum k such that 2k ≤ b_size and k ≤ a_size */
    idx_t max_k = a_size;
    idx_t half_b = b_size / 2;
    if (max_k > half_b) max_k = half_b;

    seq_t total = (seq_t)0;

    for (idx_t k = (idx_t)1; k <= max_k; ++k) {
        seq_t c1 = binom_basic(a_size, k);
        seq_t c2 = binom_basic(b_size, (idx_t)(2 * k));
        total += c1 * c2;
    }

    return total;
}
