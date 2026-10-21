/*
 * A081894  Fourth binomial transform of C(n+2,2).
 * https://oeis.org/A081894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n*(n^2 + 19*n + 50)/50 (OEIS A081894)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the two small cases explicitly to avoid excessive
       multiplication when n < 2.  The formula works for all n,
       but the loop below is only used for n >= 2. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)7;

    /* For n >= 2 we use the equivalent form
       a(n) = 5^(n-2) * ((n^2 + 19*n + 50)/2).
       The quotient is always an integer because n^2+19*n+50 is even. */
    seq_t power5 = 1;              /* 5^(n-2) */
    for (idx_t i = 0; i < n - 2; ++i)
        power5 *= (seq_t)5;

    seq_t ns = (seq_t)n;
    /* Compute (n^2 + 19*n + 50)/2.  This fits in seq_t for all n where
       a(n) fits in seq_t. */
    seq_t poly_half = (ns * ns + (seq_t)19 * ns + (seq_t)50) / (seq_t)2;

    return power5 * poly_half;
}
