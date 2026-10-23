/*
 * A170044  Number of reduced words of length n in Coxeter group on 35
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^36 = I.
 * https://oeis.org/A170044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   derived from the rational generating function of A170044 * * The
 *            generating function is * A(t) = (t^36 + 2t^35 + 2t^34 + … + 2t +
 *            1) / * (561t^36 - 33t^35 - 33t^34 - … - 33t + 1) * * Let a(n) be
 *            the coefficient of t^n. Expanding D(t)·A(t)=N(t) gives the *
 *            linear recurrence * * a(n) - 33·∑_{k=1}^{...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case immediately. */
    if (n == 0)
        return (seq_t)1;

    /* A ring buffer of the last 36 terms. */
    seq_t buf[36] = {0};
    buf[0] = (seq_t)1;   /* a(0) */
    seq_t sumPrev = buf[0];        /* sum of the preceding up to 35 terms */
    seq_t lastTerm = buf[0];       /* will hold a(n) when the loop ends */

    for (idx_t i = 1; i <= n; ++i)
    {
        /* Coefficient c(i) from the numerator */
        seq_t coeff;
        if (i <= 35)
            coeff = (seq_t)2;   /* c(1)…c(35) = 2 */
        else if (i == 36)
            coeff = (seq_t)1;   /* c(36) = 1 */
        else
            coeff = (seq_t)0;   /* c(n) = 0 for n ≥ 37 */

        /* Compute a(i) using the recurrence */
        seq_t term = coeff + (seq_t)33 * sumPrev;
        if (i >= 36)
            term -= (seq_t)561 * buf[(i - 36) % 36];

        /* Store the new term in the ring buffer */
        buf[i % 36] = term;

        /* Update the sliding window sum for the next step */
        sumPrev += term;
        if (i > 35)
            sumPrev -= buf[(i - 35) % 36];

        lastTerm = term;
    }

    return lastTerm;
}
