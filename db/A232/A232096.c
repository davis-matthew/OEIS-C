/*
 * A232096  a(n) = largest m such that m! divides 1+2+...+n; a(n) =
 *          A055881(A000217(n)).
 * https://oeis.org/A232096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest m such that m! divides n(n+1)/2
 * Range:     u8 n <= 22, u16 n <= 2623, u32 n <= 9617, u64 n <= 9617,
 *            u128 n <= 9617, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;             /* undefined below offset, return 0 */

    /* triangular number T(n) = n(n+1)/2  */
    seq_t T = (seq_t)n * ((seq_t)n + 1) / 2;

    seq_t f = 1;    /* factorial value */
    seq_t m = 0;    /* current m */

    for (;;) {
        m = m + 1;
        f = f * m;

        if (f > T)          /* cannot divide any further */
            break;

        if (T % f != 0)     /* fails divisibility */
            break;
    }

    return m - 1;        /* last successful m */
}
