/*
 * A210357  Location of the maximum modulus in the inverse of Hilbert's
 *          matrix.
 * https://oeis.org/A210357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 360, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computation of A210357: Location of the maximum modulus in the inverse Hilbert matrix.
 * The maximum always lies on the diagonal.  The diagonal entries are
 *     D(i) = (2i‑1) * C(n+i‑1, n‑i)² * C(2i‑2, i‑1)²
 * where C(a,b) is the binomial coefficient.
 * A maximum is found by testing the ratio
 *     R(i) = D(i+1) / D(i)
 * When R(i) ≤ 1 the sequence has reached its peak; the argmax is i.
 * This function implements that search using 128‑bit arithmetic to avoid overflow.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;          /* a(1) = 1 */

    for (idx_t i = 1; i < n; ++i)
    {
        /* use unsigned __int128 for intermediate products */
        unsigned __int128 ii = i;
        unsigned __int128 nn = n;

        unsigned __int128 numerator =
            (nn - ii) * (nn - ii);          /* (n-i)² */
        numerator *= (nn + ii) * (nn + ii); /* * (n+i)² */
        numerator *= (2 * ii - 1);         /* * (2i-1) */

        unsigned __int128 denom = ii * ii * ii * ii; /* i⁴ */
        denom *= (2 * ii + 1);                 /* * (2i+1) */

        if (numerator <= denom)
            return (seq_t)i;            /* maximum occurs at i */
    }

    return (seq_t)n;  /* if never decreased, maximum is at the last diagonal entry */
}
