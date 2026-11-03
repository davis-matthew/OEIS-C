/*
 * A238363  Coefficients for the commutator for the logarithm of the
 *          derivative operator [log(D),x^n D^n]=d[(xD)!/(xD-n)!]/d(xD)
 *          expanded in the operators :xD:^k.
 * https://oeis.org/A238363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = (-1)^(r−k−1) * r! / ((r−k) * k!) where r is the smallest
 *            integer with r(r+1)/2 ≥ n and k = n − (r−1)r/2 − 1. The sequence
 *            is the row‑major flattening of the triangular array a(r,k)
 *            defined above
 * Range:     i8 n <= 15, i16 n <= 28, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;             /* undefined for n < 1, return 0 safely */

    /* find the row r such that (r−1)r/2 < n ≤ r(r+1)/2 */
    idx_t r = 0;
    while ((seq_t)r * (r + 1) / 2 < n)
        ++r;

    idx_t prev = (r - 1) * r / 2;          /* T_{r-1} */
    idx_t k   = n - prev - 1;              /* 0‑based column index */

    /* compute (r!)/(k!*(r−k)) = product(i = k+1 .. r) / (r−k)  */
    seq_t prod = (seq_t)1;
    for (idx_t i = k + 1; i <= r; ++i)
        prod = mul_small(prod, (unsigned int)i);

    prod /= (seq_t)(r - k);                /* exact division */

    /* apply sign factor (-1)^(r−k−1) */
    idx_t exp = (r - k) - 1;
    if (exp & 1)
        prod = -prod;

    return prod;
}
