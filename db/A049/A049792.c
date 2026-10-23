/*
 * A049792  a(n) = Sum_{k=1..n} floor(n/floor(n/k)).
 * https://oeis.org/A049792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(n / floor(n/k))
 * Range:     u8 n <= 19, u16 n <= 318, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, if needed */

/* External, no static keyword. */
seq_t a(idx_t n)
{
    /* Sum over blocks of equal floor(n/k).  The variable `l` marks the start
     * of a block.  If floor(n/k) == v for k in [l, r], then
     *   count   = r - l + 1
     *   term    = floor(n / v)
     *   block contribution = count * term
     * We find r = floor(n / v) for the current v.
     */
    seq_t result = 0;
    if (n == 0) return 0;   /* outside of the OEIS offset, but harmless */

    idx_t l = 1;
    while (l <= n) {
        /* current value of floor(n/k) for k >= l */
        idx_t v = n / l;
        /* last index with this value */
        idx_t r = n / v;
        /* contribution of the block */
        seq_t term   = (seq_t)n / (seq_t)v;          /* floor(n / v) */
        seq_t count  = (seq_t)(r - l + 1);
        result += term * count;
        /* move to the next block */
        l = r + 1;
    }
    return result;
}
