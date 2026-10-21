/*
 * A214651  Count down from n to 1, n times.
 * https://oeis.org/A214651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = the term in the block formed by writing k,k-1,...,1
 *            repeated k times for each k=1,2,3,..., where the block
 *            containing position n is the one with cumulative length
 *            sum_{i=1}^{k} i² >= n
 * Range:     u8 n <= 450, u16 n <= 1015, u32 n <= 1015, u64 n <= 1015,
 *            u128 n <= 1015, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t cum_sq(idx_t k)
{
    seq_t kk = (seq_t)k;
    return kk * (kk + 1) * (2 * kk + 1) / 6;
}

seq_t a(idx_t n)
{
    /* find smallest k with cum_sq(k) >= n */
    idx_t k = 1;
    seq_t cum = cum_sq(k);

    while (cum < (seq_t)n) {
        ++k;
        cum = cum_sq(k);
    }

    seq_t kk   = (seq_t)k;
    seq_t prev = cum - kk * kk;          /* cumulative length before this block */
    idx_t pos  = (idx_t)(n - prev - 1);  /* 0‑based position inside the block */
    idx_t off  = pos % k;                /* offset inside one repetition of the countdown */

    return kk - (seq_t)off;
}
