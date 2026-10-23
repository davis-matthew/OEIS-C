/*
 * A115865  a(n) = Legendre_P(n,2)*6^n.
 * https://oeis.org/A115865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A115865:
 * a(0) = 1, a(1) = 12
 * recurrence (n ≥ 2):
 *    n * a(n) = 12*(2n−1) * a(n−1) − 36*(n−1) * a(n−2)
 * so a(n) = (12*(2n−1)*a(n−1) − 36*(n−1)*a(n−2)) / n
 *
 * The function computes a(n) iteratively, requiring only the two
 * previous values.  It works for any index order and enjoys O(n)
 * time and O(1) additional memory.
 */

#include <stdint.h>   /* for uint64_t if needed */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)12;

    seq_t prev2 = (seq_t)1;  /* a(0) */
    seq_t prev1 = (seq_t)12; /* a(1) */
    seq_t next;

    for (idx_t k = 2; k <= n; ++k)
    {
        /* term1 = 12 * (2k-1) * prev1 */
        seq_t twok_minus1 = (seq_t)( (seq_t)2 * (seq_t)k - (seq_t)1 );
        seq_t term1 = (seq_t)12 * (twok_minus1 * prev1);

        /* term2 = 36 * (k-1) * prev2 */
        seq_t term2 = (seq_t)36 * ( ((seq_t)k - (seq_t)1) * prev2 );

        /* a(k) = (term1 - term2) / k */
        next = (term1 - term2) / (seq_t)k;

        /* shift for next iteration */
        prev2 = prev1;
        prev1 = next;
    }

    return prev1;
}
