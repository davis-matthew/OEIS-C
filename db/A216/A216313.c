/*
 * A216313  Total number of cycles in all partial permutations of {1,2,...,n}.
 * https://oeis.org/A216313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the total number of cycles in all partial permutations
 *            of {1,2,…,n}. For n ≥ 4 the recurrence a(n) = (4n-3)a(n-1) -
 *            (6n²-17n+13)a(n-2) + (n-2)²(4n-9)a(n-3) - (n-3)³(n-2)a(n-4) with
 *            initial values a(0)=0, a(1)=1, a(2)=5, a(3)=29 is used
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the known initial values directly.  */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)29;

    /* Allocate an array of size n+1.  */
    seq_t *dp = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (dp == NULL) return (seq_t)0;          /* out of memory – will never happen in tests */

    /* Initialize the first four terms.  */
    dp[0] = (seq_t)0;
    dp[1] = (seq_t)1;
    dp[2] = (seq_t)5;
    dp[3] = (seq_t)29;

    /* Compute the terms up to n with the recurrence.  */
    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t ik = (seq_t)i;

        seq_t coeff1 = (seq_t)(4 * i - 3);
        seq_t coeff2 = (seq_t)(6 * i * i - 17 * i + 13);

        seq_t t = (seq_t)(i - 2);
        seq_t coeff3 = t * t * (seq_t)(4 * i - 9);

        t = (seq_t)(i - 3);
        seq_t coeff4 = t * t * t * (seq_t)(i - 2);

        /* Use unsigned arithmetic – intermediate subtractions are always non‑negative.  */
        seq_t term = coeff1 * dp[i - 1];
        term -= coeff2 * dp[i - 2];
        term += coeff3 * dp[i - 3];
        term -= coeff4 * dp[i - 4];

        dp[i] = term;
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
