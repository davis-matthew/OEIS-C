/*
 * A170145  Number of reduced words of length n in Coxeter group on 40
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^38 = I.
 * https://oeis.org/A170145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n ≥ 1 a(n) = 38·(a(n‑1)+a(n‑2)+…+a(n‑37)) −
 *            741·a(n‑38) + p(n) where p(1)…p(37) = 2, p(38) = 1, p(n>38) = 0
 *            The recurrence follows from the generating function (t^38 +
 *            2t^37 + … + 2t + 1)/(1 − 38t − … −38t^37 + 741t^38)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0)
        return (seq_t)1;

    /* allocate space for values 0 … n */
    seq_t *arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!arr)
        return (seq_t)0;          /* allocation failed */

    arr[0] = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i)
    {
        /* sum of the previous up to 37 terms */
        seq_t sum = 0;
        for (idx_t k = 1; k <= 37 && i >= k; ++k)
            sum += arr[i - k];

        /* main part of the recurrence */
        seq_t val = sum * 38ULL;
        if (i >= 38)
            val -= arr[i - 38] * 741ULL;

        /* add the numerator correction */
        if (i == 38)
            val += (seq_t)1;
        else if (i <= 37)
            val += (seq_t)2;

        arr[i] = val;
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
