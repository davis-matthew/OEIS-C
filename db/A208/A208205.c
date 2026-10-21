/*
 * A208205  a(n)=(a(n-1)*a(n-2)^5+1)/a(n-3) with a(0)=a(1)=a(2)=1.
 * https://oeis.org/A208205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-1)*a(n-2)^5 + 1) / a(n-3) with a(0)=a(1)=a(2)=1
 *            (OEIS A208205)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 0ULL || n == 1ULL || n == 2ULL)
        return (seq_t)1;

    /* a0 = a(n-3), a1 = a(n-2), a2 = a(n-1) at each step */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t a2 = (seq_t)1;   /* a(2) */

    seq_t a_next = 0;

    for (idx_t i = 3ULL; i <= n; ++i)
    {
        /* compute a1^5 */
        seq_t pow5 = a1;
        pow5 *= a1;   /* a1^2 */
        pow5 *= a1;   /* a1^3 */
        pow5 *= a1;   /* a1^4 */
        pow5 *= a1;   /* a1^5 */

        /* numerator = a2 * a1^5 + 1 */
        seq_t numerator = a2 * pow5 + (seq_t)1;

        /* a_next = numerator / a0 */
        a_next = numerator / a0;

        /* shift for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a_next;
    }

    return a_next;
}
