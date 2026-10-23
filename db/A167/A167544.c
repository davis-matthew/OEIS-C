/*
 * A167544  a(n) = (n-3)*(n-8)/2.
 * https://oeis.org/A167544
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = (n-3)*(n-8)/2
 * Range:     i8 n <= 21, i16 n <= 261, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 4 the sequence is not defined; return any value. */
    if (n < 4)
        return 0;

    /* Convert the unsigned index to a signed seq_t before
       performing the arithmetic.  This prevents wrap‑around
       that would occur if we subtracted first. */
    seq_t sn = (seq_t)n;
    return ((sn - 3) * (sn - 8)) / 2;
}
