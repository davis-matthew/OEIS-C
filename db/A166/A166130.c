/*
 * A166130  Number of reduced words of length n in Coxeter group on 34
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^10 = I.
 * https://oeis.org/A166130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   OEIS sequence A166130: Number of reduced words of length n in
 *   Coxeter group on 34 generators S_i with relations (S_i)^2 = (S_i S_j)^10 = I.
 *   The generating function satisfies  
 *   (1 - 32t - 32t^2 - … - 32t^9 + 528t^10) * G(t) =
 *   (t^10 + 2t^9 + 2t^8 + 2t^7 + 2t^6 + 2t^5 + 2t^4 + 2t^3 + 2t^2 + 2t + 1).
 *
 *   The recurrence for n ≥ 10 is
 *       a(n) = 32·a(n-1) + … + 32·a(n-9) - 528·a(n-10)
 *   For 0 ≤ n < 10 the terms are defined by
 *       a(n) = 32·a(n-1) + … + 32·a(n-n) + coeff_N(n)
 *   where coeff_N(n) are the numerator coefficients:
 *     coeff_N(0)=1, coeff_N(1)=…=coeff_N(9)=2, coeff_N(10)=1.
 *
 *   No dynamic allocation is needed; we keep a circular buffer of the
 *   last 10 values and recompute from the start for each call.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* numerator coefficients for degrees 0..10 */
    static const int P[11] = {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1};

    /* buffer holding the most recent 10 computed values */
    seq_t buf[10] = {0};

    seq_t value = 0;    /* value of a(i) during the loop */

    for (idx_t i = 0; i <= n; ++i)
    {
        /* base case a(0) = 1 */
        if (i == 0)
        {
            value = (seq_t)1;
        }
        else
        {
            seq_t sum = 0;

            /* add 32·a(i-k) for k = 1..9 if defined */
            for (int k = 1; k <= 9; ++k)
            {
                if (i >= (idx_t)k)
                    sum += (seq_t)32 * buf[(i - k) % 10];
            }

            /* subtract 528·a(i-10) if defined */
            if (i >= (idx_t)10)
                sum -= (seq_t)528 * buf[(i - 10) % 10];

            /* add the numerator coefficient if i ≤ 10 */
            if (i <= (idx_t)10)
                sum += (seq_t)P[i];

            value = sum;
        }

        /* store the newly computed value in the circular buffer */
        buf[i % 10] = value;
    }

    return value;
}
