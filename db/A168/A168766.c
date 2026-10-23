/*
 * A168766  Number of reduced words of length n in Coxeter group on 41
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^18 = I.
 * https://oeis.org/A168766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is defined by the rational generating function * G(t) =
 *            (t^18 + 2t^17 + 2t^16 + … + 2t + 1) / * (1 - 39t - 39t^2 - … -
 *            39t^17 + 780t^18). * This gives the linear recurrence * a(0) = 1
 *            * a(n) = 39·(a(n-1)+…+a(n-17)) - 780·a(n-18) + b(n) (n ≥ 1) *
 *            where b(n) = 2 for 1 ≤ n ≤ 17, * 1 for n = 18,...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                    /* a(0) */

    /* a circular buffer of the last 19 values a(i-18)…a(i-1) */
    seq_t buf[19] = {0};
    buf[0] = (seq_t)1;                      /* a(0) */

    const seq_t C39  = (seq_t)39;
    const seq_t C780 = (seq_t)780;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = 0;

        /* sum a(i-1) … a(i-17), zero if index < 0 */
        idx_t maxk = (i < 17 ? i : 17);
        for (idx_t k = 1; k <= maxk; ++k) {
            idx_t idx = (i - k) % 19;
            sum += buf[idx];
        }

        /* determine the numerator coefficient b(i) */
        seq_t b = 0;
        if (i <= 17)
            b = (seq_t)2;
        else if (i == 18)
            b = (seq_t)1;

        seq_t val = C39 * sum + b;

        if (i >= 18) {
            idx_t idx18 = (i - 18) % 19;
            val -= C780 * buf[idx18];
        }

        buf[i % 19] = val;
    }

    return buf[n % 19];
}
