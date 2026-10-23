/*
 * A015772  Inverse of 1763rd cyclotomic polynomial.
 * https://oeis.org/A015772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   a(n) = 1 for n mod 1763 in [0,40]; a(n) = 0 for n mod 1763 in
 *            [41,42]; a(n) = -1 for n mod 1763 in [43,83]; a(n) = 0
 *            otherwise. This gives the period‑1763 sequence of the inverse of
 *            the 1763rd cyclotomic polynomial
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t t = n % 1763ULL;          /* 1763ULL is safe for all idx_t sizes */

    if (t < 41ULL)
        return (seq_t)1;
    else if (t < 43ULL)
        return (seq_t)0;
    else if (t < 84ULL)
        return (seq_t)-1;
    else
        return (seq_t)0;
}
