/*
 * A170110  Number of reduced words of length n in Coxeter group on 5
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^38 = I.
 * https://oeis.org/A170110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the linear recurrence for OEIS A170110.
 * From the generating function G(t) we obtain
 *   a(0) = 1;
 *   for n ≥ 1
 *     a(n) = 3 * Σ_{i=1}^{min(37,n)} a(n-i)
 *            - 6 * a(n-38)  (if n ≥ 38)
 *            + b(n)
 *   where b(0)=1, b(1)…b(37)=2, b(38)=1, and b(n)=0 for n>38.
 *
 * The recurrence is implemented straightforwardly: for each n we
 * compute the sum of the last up to 37 terms, add the appropriate
 * constant, subtract 6 * a(n‑38) when needed, and store the result.
 * All arithmetic is performed in the type seq_t, so the code works
 * for 8‑, 16‑, 32‑, 64‑bit unsigned integers and arbitrary‑precision
 * integers.  The result is computed in O(n) time and O(n) memory.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Allocate array for a[0..n] */
    seq_t *arr = (seq_t *)malloc((n + 1ULL) * sizeof(seq_t));
    if (!arr)            /* unlikely, but safe */
        return (seq_t)0;

    arr[0] = (seq_t)1;

    for (idx_t k = 1; k <= n; ++k) {
        idx_t lim = (k < 37ULL ? k : 37ULL);
        seq_t sum = 0;
        for (idx_t i = 1; i <= lim; ++i)
            sum += arr[k - i];

        seq_t val = 3 * sum;
        if (k >= 38ULL)
            val -= 6 * arr[k - 38ULL];

        seq_t add = 0;
        if (k <= 37ULL)
            add = 2;
        else if (k == 38ULL)
            add = 1;
        val += add;

        arr[k] = val;
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
