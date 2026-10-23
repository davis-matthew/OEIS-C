/*
 * A170639  Number of reduced words of length n in Coxeter group on 6
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
 * https://oeis.org/A170639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (t^49 + 2*t^48 + ... + 2*t + 1) / (1 - 4*t - 4*t^2 - ...
 *            - 4*t^48 + 10*t^49) Recurrence: for n >= 0 a(n) = p(n) +
 *            4*(a(n-1)+...+a(n-48)) - 10*a(n-49), where p(0)=1, p(49)=1,
 *            p(1..48)=2 and a(k)=0 for k<0. Implementation uses a circular
 *            buffer of size 50. @param n 0‑based index @return the...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Circular buffer for the last 50 values. */
    seq_t last[50] = {0};
    seq_t p, sum;
    idx_t i, k;
    for (i = 0; i <= n; ++i) {
        /* compute p(i) */
        if (i == 0 || i == 49)
            p = 1;
        else
            p = 2;

        sum = 0;
        /* add 4 * a(i-1) ... 4 * a(i-48) */
        for (k = 1; k <= 48; ++k) {
            if ((idx_t)i >= k) {
                seq_t tmp = last[(i - k) % 50];
                /* multiply by 4 */
                tmp = tmp * (seq_t)4;
                sum = sum + tmp;
            }
        }
        /* subtract 10 * a(i-49) if i >= 49 */
        if ((idx_t)i >= 49) {
            seq_t tmp = last[(i - 49) % 50];
            tmp = tmp * (seq_t)10;
            sum = sum - tmp;
        }
        /* compute a(i) */
        seq_t ai = p + sum;
        /* store in buffer */
        last[i % 50] = ai;
    }
    return last[n % 50];
}
