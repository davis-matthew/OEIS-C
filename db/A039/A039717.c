/*
 * A039717  Row sums of convolution triangle A030523.
 * https://oeis.org/A039717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 1, a(2) = 4, a(n) = 5*a(n-1) - 5*a(n-2) (n >= 3)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term of sequence A039717.  */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;        /* undefined for n < 1, return 0 */

    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)4;

    /* Compute iteratively using the recurrence. */
    seq_t prev2 = (seq_t)1;   /* a(1) */
    seq_t prev1 = (seq_t)4;   /* a(2) */
    seq_t cur = prev1;        /* placeholder for a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)5 * prev1 - (seq_t)5 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
