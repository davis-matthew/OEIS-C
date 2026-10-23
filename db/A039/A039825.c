/*
 * A039825  a(n) = floor((n^2 + n + 8) / 4).
 * https://oeis.org/A039825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2 + n + 8) / 4)
 * Range:     u8 n <= 31, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1  (i.e. a(1) = 2) */

seq_t a(idx_t n)
{
    /* Convert to seq_t to keep type consistent */
    seq_t sn = (seq_t)n;
    /* Compute the formula.  Integer division truncates towards zero,
       which for unsigned integers is equivalent to floor. */
    return (sn * sn + sn + 8) / 4;
}
