/*
 * A169051  Number of reduced words of length n in Coxeter group on 38
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^24 = I.
 * https://oeis.org/A169051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1. For 1 ≤ n < 24: a(n) = 2 + 36 * Σ_{k=1}^{n} a(n-k).
 *            For n ≥ 24: a(n) = 36 * Σ_{k=1}^{23} a(n-k) – 666 * a(n-24).
 *            (Derived from the rational generating function
 *            (t^24+2t^23+…+2t+1)/(666t^24−36t^23−…−36t+1).)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Small circular buffer holding the last 24 terms. */
    seq_t buf[24];
    seq_t total_sum = 0;          /* sum of all elements currently in buf   */
    seq_t a_i;                    /* value for current index                */
    idx_t pos = 0;                /* write position in buf                  */
    idx_t i;

    for (i = 0; i <= n; ++i) {
        if (i == 0) {
            a_i = 1;
            total_sum = a_i;               /* buf[0] will be set below            */
        } else if (i < 24) {
            a_i = 2 + (seq_t)36 * total_sum;
            total_sum += a_i;
        } else { /* i >= 24 */
            seq_t old   = buf[pos];        /* a(i-24)                               */
            seq_t sum23 = total_sum - old; /* a(i-1) + … + a(i-23)                 */
            a_i = (seq_t)36 * sum23 - (seq_t)666 * old;
            total_sum = total_sum - old + a_i;
        }

        buf[pos] = a_i;
        pos = (pos + 1) % 24;

        if (i == n)
            return a_i;
    }
    /* control never reaches here */
    return 0;
}
