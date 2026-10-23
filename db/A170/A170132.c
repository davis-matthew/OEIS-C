/*
 * A170132  Number of reduced words of length n in Coxeter group on 27
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^38 = I.
 * https://oeis.org/A170132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n ≥ 1: a(n) = 25 * (a(n-1)+a(n-2)+…+a(n-37)) - 325
 *            * a(n-38) + c(n) with c(n) = 2 if 1 ≤ n ≤ 37, c(n) = 1 if n =
 *            38, c(n) = 0 if n > 38. This implements the recurrence coming
 *            from the generating function
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    size_t sz = (size_t)n + 1;
    seq_t *arr = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!arr) return 0;         /* out of memory – graceful fail */

    arr[0] = 1;
    seq_t sumWindow = 0;        /* sum of the last up to 37 previous terms */

    for (idx_t i = 1; i <= n; ++i) {
        sumWindow += arr[i-1];          /* add the new previous term */

        if (i >= 38)                    /* keep only the last 37 terms */
            sumWindow -= arr[i-38];

        seq_t const_term = 0;
        if (i == 38)          const_term = 1;
        else if (i <= 37)     const_term = 2;

        seq_t val = (seq_t)25 * sumWindow + const_term;

        if (i >= 38)
            val = val - (seq_t)325 * arr[i-38];

        arr[i] = val;
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
