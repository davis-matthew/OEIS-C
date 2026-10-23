/*
 * A170566  Number of reduced words of length n in Coxeter group on 29
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^47 = I.
 * https://oeis.org/A170566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, * a(n)=27*(a(n-1)+⋯+a(n-46))−378*a(n-47)+p(n) * where
 *            p(0)=1, p(1)…p(46)=2, p(47)=1, p(n)=0 for n>47. * The recurrence
 *            follows from the generating function of * sequence A170566
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Window holding the last 47 terms a(i-47) … a(i-1) */
    seq_t last[47] = {0};
    seq_t sum46   = 0;    /* sum of a(i-1) … a(i-46) */
    seq_t term    = 0;    /* a(i) being computed */

    for (idx_t i = 0; i <= n; ++i)
    {
        size_t idx = (size_t)(i % 47);          /* position of a(i-47) in the window */

        /* p(i) */
        seq_t p;
        if (i == 0)
            p = (seq_t)1;
        else if (i <= 46)
            p = (seq_t)2;
        else if (i == 47)
            p = (seq_t)1;
        else
            p = (seq_t)0;

        seq_t a_minus47 = last[idx];                     /* a(i-47)   */
        seq_t a_minus46 = last[(idx + 1) % 47];           /* a(i-46)   */

        /* a(i) = 27*sum46 + p - 378*a_minus47 */
        term = (seq_t)27 * sum46 + p - (seq_t)378 * a_minus47;

        /* Update sum46 for the next iteration:
         * sum46' = sum46 - a(i-46) + a(i)
         */
        sum46 = sum46 - a_minus46 + term;

        /* Place a(i) into the window, overwriting a(i-47) */
        last[idx] = term;
    }

    return term;
}
