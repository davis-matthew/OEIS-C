/*
 * A015952  Inverse of 1943rd cyclotomic polynomial.
 * https://oeis.org/A015952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 for n mod 1943 in [0,28]; a(n) = 0 for n mod 1943 in
 *            [29,66] or [96,1942]; a(n) = -1 for n mod 1943 in [67,95]
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A015952. */
seq_t a(idx_t n)
{
    const idx_t period = 1943;
    idx_t r = n % period;
    if (r <= 28)
        return (seq_t)1;
    else if (r <= 66)
        return (seq_t)0;
    else if (r <= 95)
        return (seq_t)-1;
    else
        return (seq_t)0;
}
