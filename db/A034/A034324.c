/*
 * A034324  a(n) = (n-1)*(n-2)*(n-3) + n.
 * https://oeis.org/A034324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n-1)*(n-2)*(n-3) + n
 * Range:     u8 n <= 8, u16 n <= 42, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1  (a(1) is the first term listed) */
#include <stdint.h>

/* Compute the n-th term of OEIS A034324 */
seq_t a(idx_t n)
{
    /* For n = 1, 2, 3 the product is zero, so a(n) = n */
    if (n <= 3) {
        return (seq_t)n;
    }

    /* For n >= 4 we can safely use unsigned arithmetic */
    seq_t sn = (seq_t)n;
    return (sn - 1U) * (sn - 2U) * (sn - 3U) + sn;
}
