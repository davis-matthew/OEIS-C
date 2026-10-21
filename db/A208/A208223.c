/*
 * A208223  a(n) = (a(n-1)*a(n-3)^3+a(n-2))/a(n-4) with a(0)=a(1)=a(2)=a(3)=1.
 * https://oeis.org/A208223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-1)*a(n-3)^3 + a(n-2)) / a(n-4) with a(0) = a(1) =
 *            a(2) = a(3) = 1
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 10, u64 n <= 11, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* Small n: all terms are 1 */
    if (n < 4)
        return (seq_t)1;

    /* Keep the last four values: a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t a0 = (seq_t)1;  /* a(0) */
    seq_t a1 = (seq_t)1;  /* a(1) */
    seq_t a2 = (seq_t)1;  /* a(2) */
    seq_t a3 = (seq_t)1;  /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute a(n-3)^3: a1^3 */
        seq_t tmp = a1 * a1;
        tmp *= a1;                /* now tmp = a1^3 */

        /* numerator = a(n-1) * a(n-3)^3 + a(n-2) */
        seq_t numerator = a3 * tmp + a2;

        /* a(n) = numerator / a(n-4); division is exact by definition */
        seq_t next = numerator / a0;

        /* shift the window: a0 <- a1, a1 <- a2, a2 <- a3, a3 <- next */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    /* a3 now holds a(n) */
    return a3;
}
