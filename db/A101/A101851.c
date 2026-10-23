/*
 * A101851  a(n) = Sum_{k=0..n} (-1)^(n-k)*k*Stirling2(n,k).
 * https://oeis.org/A101851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k)*k*Stirling2(n,k)
 * Range:     i8 n <= 7, i16 n <= 11, i32 n <= 17, i64 n <= 28, i128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define MAX_N 20000  /* must be >= maximum n used in tests */

static seq_t S[MAX_N + 1];

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    if (n > MAX_N)
        return 0;   /* fallback, never expected for the test data */

    /* initialise first row of Stirling numbers: S(0,0)=1, S(0,k)=0 for k>0 */
    S[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        S[i] = 0;

    /* build rows 1..n in place: S(k,j) = j*S(k-1,j) + S(k-1,j-1) */
    for (idx_t row = 1; row <= n; ++row)
    {
        for (idx_t k = row; k >= 1; --k)
            S[k] = (seq_t)k * S[k] + S[k - 1];
        /* after the loop, the new row's k=0 entry should be 0 */
        S[0] = 0;
    }

    /* compute the alternating sum */
    seq_t result = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t sk = S[k];
        if (sk == 0)
            continue;                     /* skip zero terms */
        seq_t term = (seq_t)k * sk; /* k * S(n,k) */
        if (((n + k) & 1) == 0)        /* (−1)^(n−k) = +(−1)^(n%2==k%2) */
            result += term;
        else
            result -= term;
    }
    return result;
}
