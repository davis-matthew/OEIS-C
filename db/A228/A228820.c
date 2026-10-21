/*
 * A228820  Sum of positive F-ranks of all compositions of n. Also, sum of
 *          positive L-ranks of all compositions of n (see comments lines for
 *          definition).
 * https://oeis.org/A228820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{f=1}^{n} Σ_{m=1}^{min(n-f, 2^{f-1}-2)} binom(n-f-1,
 *            m-1) · (2^{f-1}-1-m) (sum of positive F‑ranks of all
 *            compositions of n)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 60, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)0;

    /* pre‑compute powers of two up to n */
    seq_t *pow2 = (seq_t *)malloc((n + 1u) * sizeof(seq_t));
    if (!pow2)
        return (seq_t)0;   /* allocation failure – unlikely in test harness */

    pow2[0] = (seq_t)1;
    for (idx_t i = 1u; i <= n; ++i)
        pow2[i] = pow2[i - 1] * (seq_t)2;

    seq_t sum = (seq_t)0;

    for (idx_t f = 1u; f <= n; ++f)
    {
        idx_t r = n - f;          /* sum of the remaining parts */

        if (r == 0u)              /* composition consists only of f */
        {
            sum += pow2[f - 1] - (seq_t)1;
            continue;
        }

        /* for positive ranks we need 2^(f-1) ≥ 3 */
        if (pow2[f - 1] < (seq_t)3)
            continue;

        /* maximum number of parts in the remainder that keep the rank positive */
        idx_t maxm = (r < (idx_t)(pow2[f - 1] - (seq_t)2)) ?
                     r : (idx_t)(pow2[f - 1] - (seq_t)2);

        if (maxm == r)          /* all compositions of r are allowed */
        {
            if (r >= 2u)
            {
                seq_t term = pow2[r - 2];                 /* 2^{r-2} */
                seq_t mult = pow2[f] - (seq_t)r - (seq_t)3; /* 2^f - r - 3 */
                sum += term * mult;
            }
            else /* r == 1 */
            {
                seq_t rank = pow2[f - 1] - (seq_t)2; /* only one composition [1] */
                sum += rank;                        /* rank is positive here */
            }
        }
        else                    /* partial sum over m = 1 … maxm */
        {
            seq_t comb = (seq_t)1;                     /* C(r-1,0) */
            for (idx_t m = 1u; m <= maxm; ++m)
            {
                seq_t rank = pow2[f - 1] - (seq_t)1 - (seq_t)m;
                sum += comb * rank;

                if (m < maxm)
                {
                    /* update comb to C(r-1, m) */
                    comb = (comb * (seq_t)(r - m)) / (seq_t)m;
                }
            }
        }
    }

    free(pow2);
    return sum;
}
