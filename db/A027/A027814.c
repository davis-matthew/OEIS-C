/*
 * A027814  a(n) = 126*(n+1)*binomial(n+5,9)/5.
 * https://oeis.org/A027814
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 126*(n+1)*binomial(n+5,9)/5
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 23, u64 n <= 219, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* multiplicative binomial for small k (k = 9 in this sequence) */
static seq_t binom_small(idx_t N, idx_t K)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= K; ++i) {
        res *= (seq_t)(N - K + i);
        res /= (seq_t)i;          /* exact division */
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 4) return 0;          /* sequence offset is 4 */
    /* compute binomial (n+5,9) */
    seq_t bin = binom_small(n + 5, 9);
    seq_t res = bin;
    res *= (seq_t)(n + 1);
    res /= (seq_t)5;              /* division is exact */
    res *= 126;
    return res;
}
