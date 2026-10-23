/*
 * A014314  Number of up steps in all length n left factors of Dyck paths.
 * https://oeis.org/A014314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=ceil(n/2)}^n i * (2*i - n + 1)/(i+1) * C(n,i) *
 *            where C(n,i) is the binomial coefficient. * This is a direct
 *            summation that uses only integer operations on the *
 *            user‑supplied types seq_t and idx_t, so it works for both
 *            fixed‑width * and arbitrary‑precision builds. It runs in O(n)
 *            t...
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 62, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* lower limit of the sum (ceil(n/2)) */
    idx_t start = (n + 1) / 2;

    seq_t sum   = 0;        /* running total of up steps       */
    seq_t binom = 1;        /* C(n,i), initially C(n,0) = 1   */

    for (idx_t i = 0; i <= n; ++i)
    {
        /* update binomial coefficient: C(n,i) = C(n,i-1) * (n-i+1)/i  */
        if (i > 0)
            binom = (seq_t)((seq_t)binom * (n - i + 1) / i);

        if (i >= start)
        {
            /* term = i * (2*i - n + 1)/(i+1) * C(n,i)           */
            seq_t num     = (seq_t)(2 * i - n + 1);          /* numerator  */
            seq_t temp    = binom * num;                    /* binom * num      */
            seq_t temp_div = temp / (i + 1);                /* divide by (i+1)  */
            seq_t term     = temp_div * i;                  /* multiply by i    */
            sum += term;
        }
    }
    return sum;
}
