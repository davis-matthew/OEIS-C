/*
 * A169501  Number of reduced words of length n in Coxeter group on 8
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^34 = I.
 * https://oeis.org/A169501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1 + 2t + 2t^2 + … + 2t^33 + t^34) / (1 - 6t - 6t^2 - … -
 *            6t^33 + 21t^34)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) satisfies a_n = N_n + 6 * Σ_{k=1}^{min(n,33)} a_{n-k} - 21 * a_{n-34} (a_{m}=0 for m<0)  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* allocate space for a[0..n] */
    seq_t *a = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!a) return 0;   /* fallback on allocation failure */

    for (idx_t i = 0; i <= n; ++i)
    {
        /* numerator coefficient N(i) */
        seq_t nin;
        if (i == 0)
            nin = (seq_t)1;
        else if (i <= 33)
            nin = (seq_t)2;
        else if (i == 34)
            nin = (seq_t)1;
        else
            nin = (seq_t)0;

        /* compute Σ_{k=1}^{min(i,33)} a_{i-k} */
        seq_t sum = 0;
        for (idx_t k = 1; k <= i && k <= 33; ++k)
            sum += a[i - k];

        /* a_i = N_i + 6*sum - 21*a_{i-34} (last term only when i>=34) */
        seq_t val = nin + sum * (seq_t)6;
        if (i >= 34)
            val -= a[i - 34] * (seq_t)21;

        a[i] = val;
    }

    seq_t result = a[n];
    free(a);
    return result;
}
