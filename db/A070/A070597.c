/*
 * A070597  Duplicate of A070474.
 * https://oeis.org/A070597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n^5 mod 12
 * Range:     i8 n <= 99, i16 n <= 99, i32 n <= 99, i64 n <= 99, i128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce the base modulo 12 first. */
    idx_t m = n % 12;

    seq_t r = 1;
    /* Compute m^5 modulo 12 by repeated multiplication. */
    for (int i = 0; i < 5; ++i)
    {
        /* (r * m) stays within 0..121, well within seq_t range. */
        r = (seq_t)((seq_t)r * (seq_t)m % 12);
    }

    return r;
}
