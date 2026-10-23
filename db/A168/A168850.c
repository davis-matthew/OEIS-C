/*
 * A168850  Number of reduced words of length n in Coxeter group on 29
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^20 = I.
 * https://oeis.org/A168850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n) = 27⋅(a(n-1)+⋯+a(n-19)) – 378⋅a(n-20) + rhs(n)
 *            (n≥1) rhs(0)=1, rhs(1 … 19)=2, rhs(20)=1, rhs(n>20)=0 (the “rhs”
 *            term comes from the numerator of the rational G.f.)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* we need all values up to n; array size n+1 is safe for n≤10000 */
    seq_t *seq = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!seq)
        return (seq_t)0;          /* allocation failed */

    seq[0] = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t sum_prev = 0;
        /* sum the last 19 entries (or fewer if i<19) */
        for (int k = 1; k <= 19 && i >= (idx_t)k; ++k)
            sum_prev += seq[i - k];

        seq_t term = (seq_t)27 * sum_prev;

        if (i >= 20)
            term -= (seq_t)378 * seq[i - 20];

        /* add the rhs contribution from the numerator of the G.f. */
        if (i == 0)
            term += 1;
        else if (i <= 19)
            term += 2;
        else if (i == 20)
            term += 1;
        /* else rhs = 0 */

        seq[i] = term;
    }

    seq_t result = seq[n];
    free(seq);
    return result;
}
