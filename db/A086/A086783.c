/*
 * A086783  Discriminant of the polynomial x^n - 1.
 * https://oeis.org/A086783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^floor((n-1)/2) * n^n (offset 1)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 15, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 1. For any other value
       the behaviour is unspecified; return 0 in that case. */
    if (n == 0)
        return 0;

    /* Compute n^n.  The result is stored in a variable of type seq_t
       so any overflow behavior follows the semantics of seq_t (e.g.
       modulo for fixed-width types, exact arithmetic for arbitrary-precision). */
    seq_t res = 1;
    for (idx_t i = 0; i < n; ++i)
        res *= (seq_t)n;

    /* Determine the sign: (-1)^(floor((n-1)/2)).  When floor((n-1)/2)
       is odd the sign is negative. */
    idx_t k = (n - 1) / 2;
    if (k & 1)
        res = -res;

    return res;
}
