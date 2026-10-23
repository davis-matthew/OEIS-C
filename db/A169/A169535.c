/*
 * A169535  Number of reduced words of length n in Coxeter group on 42
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^34 = I.
 * https://oeis.org/A169535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0) = 1;
 * for 1 ≤ n ≤ 33:       a(n) = 2 + 40·∑_{k=0}^{n-1} a(k);
 * for n = 34:           a(34) = 1 + 40·∑_{k=1}^{33} a(k) – 820·a(0);
 * for n ≥ 35:           a(n) = 40·∑_{k=n-33}^{n-1} a(k) – 820·a(n-34);
 */
#include <stdlib.h>
#include <stddef.h>

/*
 * The sequence is defined by a rational generating function whose
 * coefficients satisfy the linear recurrence derived above.
 * The implementation computes the first n terms consecutively
 * (without memoisation) and returns a(n).  All integer constants
 * are cast to seq_t to avoid accidental truncation.
 */
seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0)
        return (seq_t)1;

    /* allocate array a[0..n] */
    seq_t *arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!arr)
        return (seq_t)0;              /* fallback on allocation failure */

    /* initial term */
    arr[0] = (seq_t)1;

    /* compute terms 1..33 */
    seq_t cumulative = arr[0];        /* ∑_{k=0}^{i-1} a(k) */
    for (idx_t i = 1; i <= n && i <= 33; ++i)
    {
        arr[i] = (seq_t)2 + (seq_t)40 * cumulative;
        cumulative += arr[i];
    }

    /* compute term 34 if needed */
    if (n >= 34)
    {
        seq_t sum_prev33 = (seq_t)0;
        for (idx_t k = 1; k <= 33; ++k)
            sum_prev33 += arr[k];
        arr[34] = (seq_t)1 + (seq_t)40 * sum_prev33 - (seq_t)820 * arr[0];
    }

    /* compute terms ≥ 35 */
    for (idx_t i = 35; i <= n; ++i)
    {
        seq_t sum_last33 = (seq_t)0;
        for (idx_t k = i - 33; k < i; ++k)
            sum_last33 += arr[k];
        arr[i] = (seq_t)40 * sum_last33 - (seq_t)820 * arr[i - 34];
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
