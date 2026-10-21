/*
 * A065947  Bessel polynomial {y_n}''(3).
 * https://oeis.org/A065947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = y_n''(3), where y_0(x)=1, y_1(x)=x+1, recurrence
 *            y_{n+1}(x) = (2n+1) * x * y_n(x) + y_{n-1}(x). The values
 *            a(0)=0, a(1)=0 are obtained from the second derivative
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* special cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;

    /* constants */
    const seq_t X3 = (seq_t)3;
    const seq_t ONE = (seq_t)1;

    /* start values */
    seq_t aprev = ONE;                /* y_0(3) = 1 */
    seq_t acurr = X3 + ONE;           /* y_1(3) = 1 + 3 = 4 */

    seq_t bprev = (seq_t)0;           /* y_0'(3) = 0 */
    seq_t bcurr = ONE;                /* y_1'(3) = 1 */

    seq_t cprev = (seq_t)0;           /* y_0''(3) = 0 */
    seq_t ccurr = (seq_t)0;           /* y_1''(3) = 0 */

    for (idx_t m = 1; m < n; ++m) {
        /* factor (2m+1) as seq_t */
        seq_t fact = (seq_t)(2 * m + 1);

        /* compute y_{m+1}''(3) */
        seq_t newC = fact * ((seq_t)2 * bcurr + X3 * ccurr) + cprev;

        /* compute y_{m+1}'(3) */
        seq_t newB = fact * (acurr + X3 * bcurr) + bprev;

        /* compute y_{m+1}(3)   */
        seq_t newA = fact * (X3 * acurr) + aprev;

        /* shift previous values for next iteration */
        cprev = ccurr; ccurr = newC;
        bprev = bcurr; bcurr = newB;
        aprev = acurr; acurr = newA;
    }

    return ccurr;   /* a(n) = y_n''(3) */
}
