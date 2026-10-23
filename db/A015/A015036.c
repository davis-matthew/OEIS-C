/*
 * A015036  Inverse of 1027th cyclotomic polynomial.
 * https://oeis.org/A015036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n mod 1027 ∈ [0,12]; * a(n) = -1 if n mod 1027 ∈
 *            [79,91]; * 0 otherwise. * This follows from 1/Φ_1027(x) =
 *            ((x^13-1)/(x-1))*((x^79-1)/(x^1027-1)
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute residue modulo 1027.  The modulus is small enough to fit in idx_t. */
    idx_t r = n % 1027U;

    if (r <= 12U)
        return (seq_t)1;
    if (r >= 79U && r <= 91U)
        return (seq_t)-1;
    return (seq_t)0;
}
