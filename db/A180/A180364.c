/*
 * A180364  a(n) = sum_{k=0..n} C(n,k)*C(n+k,k)*(2*k+1)^2, where C(m,k)
 *          denotes the binomial coefficient m!/(k!*(m-k)!).
 * https://oeis.org/A180364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = sum_{k=0..n} C(n,k) * C(n+k,k) * (2*k+1)^2
 *
 * This implementation computes the sum directly by iterating over
 * k and maintaining the two binomial coefficients C(n,k) and
 * C(n+k,k) multiplicatively.  All arithmetic is performed on
 * type seq_t, so the function works for the 8/16/32/64/128‑bit
 * integer builds as well as the arbitrary‑precision build.
 */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Special case n = 0 -> 1 (the sum is C(0,0)*C(0,0)*1^2) */
    if (n == (idx_t)0)
        return (seq_t)1;

    seq_t sum = 0;
    seq_t C1   = (seq_t)1; /* C(n,0) */
    seq_t C2   = (seq_t)1; /* C(n+0,0) = 1 */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* (2*k+1) as seq_t */
        seq_t tmp = (seq_t)( (seq_t)2 * k + (seq_t)1 );

        /* term = C1 * C2 * tmp^2 */
        seq_t term = C1 * C2;
        term *= tmp;
        term *= tmp;

        sum += term;

        /* Update the binomial coefficients for the next k */
        if (k < n)
        {
            /* C(n,k+1) = C(n,k) * (n-k)/(k+1) */
            C1 = (C1 * ((seq_t)n - k)) / ((seq_t)k + (seq_t)1);

            /* C(n+k+1,k+1) = C(n+k,k) * (n+k+1)/(k+1) */
            C2 = (C2 * ((seq_t)n + k + (seq_t)1)) / ((seq_t)k + (seq_t)1);
        }
    }
    return sum;
}
