/*
 * A169192  Number of reduced words of length n in Coxeter group on 35
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^27 = I.
 * https://oeis.org/A169192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 33*a(n-1)+33*a(n-2)+...+33*a(n-26)-561*a(n-27)+p_n *
 *            where p_0 = 1, p_i = 2 for 1 <= i <= 26, p_27 = 1, p_n = 0 for n
 *            > 27. * This recurrence is derived from the generating function
 *            * (t^27 + 2*t^26 + ... + 2*t + 1) / (1 - 33*t - 33*t^2 - ... -
 *            33*t^26 + 561*t^27)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* buffer holding last 27 computed values */
    seq_t buf[27] = {0};
    seq_t res = 0;

    for (idx_t i = 0; i <= n; ++i) {
        seq_t sum = 0;

        /* add 33 times the previous 26 values */
        for (int k = 1; k <= 26 && i >= k; ++k) {
            sum += (seq_t)33 * buf[(i - k) % 27];
        }

        /* subtract 561 times the value 27 steps back, if available */
        if (i >= 27) {
            sum -= (seq_t)561 * buf[(i - 27) % 27];
        }

        /* add the numerator coefficient p_i */
        if (i == 0) {
            sum += 1;
        } else if (i <= 26) {
            sum += 2;
        } else if (i == 27) {
            sum += 1;
        }

        res = sum;
        buf[i % 27] = res;
    }

    return res;
}
