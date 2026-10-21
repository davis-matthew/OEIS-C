/*
 * A230307  a(n) = 139*n^2 - 2307*n + 3331.
 * https://oeis.org/A230307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 139*n^2 - 2307*n + 3331
 * Range:     i8 none, i16 n <= 25, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast the index to the value type before any arithmetic to avoid
       unintended integer promotion and to allow the calculation to be
       performed in the appropriate precision (e.g., 64‑bit, 128‑bit,
       or arbitrary length). */
    seq_t nn = (seq_t)n;

    /* Compute the quadratic polynomial directly.  The operations are
       performed in the seq_t type, so any intermediate carries are
       handled correctly for the chosen implementation. */
    seq_t result = 139 * nn * nn - 2307 * nn + 3331;

    return result;
}
